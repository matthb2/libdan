/*
    Copyright 2012 Dan Ibanez
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
    http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "dan.h"
#include "dan_pmsg.h"

enum { sending, receiving };

void dan_pmsg_init(dan_pmsg* b)
{
    dan_pmsg temp = DAN_PMSG_INIT;
    *b = temp;
    b->state = sending;
    b->ibarrier.tag = DAN_PMSG_IBARRIER_TAG;
}

static void free_peers(dan_aa_tree* t)
{
    if (*t == &dan_aa_bottom)
        return;
    free_peers(&((*t)->left));
    free_peers(&((*t)->right));
    dan_pmsg_peer* peer;
    peer = (dan_pmsg_peer*) *t;
    dan_free_message(&(peer->message));
    dan_free(peer);
    *t = &dan_aa_bottom;
}

void dan_pmsg_start(dan_pmsg* b, dan_pmsg_method method)
{
    dan_pmsg_free(b);
    b->state = sending;
    b->method = method;
}

static bool peer_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((dan_pmsg_peer*)a)->message.peer
         < ((dan_pmsg_peer*)b)->message.peer;
}

static dan_pmsg_peer* find_peer(dan_aa_tree t, int id)
{
    dan_pmsg_peer key;
    key.message.peer = id;
    return (dan_pmsg_peer*) dan_aa_find((dan_aa_node*)&key,t,peer_less);
}

void* dan_pmsg_pack(dan_pmsg* m, int id, size_t size)
{
    dan_pmsg_peer* peer = find_peer(m->peers,id);
    if (!peer)
    {
        peer = dan_malloc(sizeof(*peer));
        dan_pmsg_peer temp = DAN_PMSG_PEER_INIT;
        *peer = temp;
        peer->message.peer = id;
        dan_aa_insert((dan_aa_node*)peer,&(m->peers),peer_less);
    }
    return dan_push_buffer2(&(peer->message.buffer),size);
}

bool dan_pmsg_has_peer(dan_pmsg* m, int id)
{
    return find_peer(m->peers,id);
}

size_t dan_pmsg_packed(dan_pmsg* m, int id)
{
    dan_pmsg_peer* peer = find_peer(m->peers,id);
    DAN_FAIL_IF(!peer,"there is no peer for this peer id. use dan_pmsg_pack to allocate a peer first.")
    return peer->message.buffer.size;
}

static void send_peers(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return;
    send_peers(t->left);
    dan_pmsg_peer* peer;
    peer = (dan_pmsg_peer*)t;
    dan_send(&(peer->message),DAN_PMSG_TAG);
    send_peers(t->right);
}

void dan_pmsg_send(dan_pmsg* m)
{
    send_peers(m->peers);
}

static bool done_sending_peers(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return true;
    dan_pmsg_peer* peer;
    peer = (dan_pmsg_peer*)t;
    return dan_done(&(peer->message))
        && done_sending_peers(t->left) && done_sending_peers(t->right);
}

static bool done_receiving_peers(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return true;
    dan_pmsg_peer* peer;
    peer = (dan_pmsg_peer*)t;
    return peer->received_from
        && done_receiving_peers(t->left) && done_receiving_peers(t->right);
}

bool dan_pmsg_receive(dan_pmsg* m)
{
    if (m->method == dan_pmsg_local)
    {
        if (done_receiving_peers(m->peers))
        {
            while(!done_sending_peers(m->peers));
            return false;
        }
    }
    m->received.peer = MPI_ANY_SOURCE;
    while (!dan_receive(&(m->received),DAN_PMSG_TAG))
    {
        if (m->method == dan_pmsg_global)
        {
            if (m->state == sending)
            {
                if (done_sending_peers(m->peers))
                {
                    dan_mpi_begin_ibarrier(&(m->ibarrier),m->ibarrier.tag);
                    m->state = receiving;
                }
            }
            else if (dan_mpi_ibarrier_done(&(m->ibarrier)))
            {
                dan_mpi_begin_ibarrier(&(m->ibarrier),m->ibarrier.tag);
                return false;
            }
        }
    }
    if (m->method == dan_pmsg_local)
    {
        dan_pmsg_peer* peer = find_peer(m->peers,m->received.peer);
        DAN_FAIL_IF(!peer,"received from a peer not sent to using local method")
        peer->received_from = true;
    }
    dan_begin_buffer2(&(m->received.buffer));
    return true;
}

void* dan_pmsg_unpack(dan_pmsg* m, size_t size)
{
    return dan_walk_buffer2(&(m->received.buffer),size);
}

int dan_pmsg_received_from(dan_pmsg* m)
{
    return m->received.peer;
}

size_t dan_pmsg_received_size(dan_pmsg* m)
{
    return m->received.buffer.size;
}

void dan_pmsg_free(dan_pmsg* b)
{
    /* ensures no one starts a new superstep while others are receiving
       in the past superstep */
    if ((b->method == dan_pmsg_global)&&(b->state == receiving))
        while (!dan_mpi_ibarrier_done(&(b->ibarrier)));
    free_peers(&(b->peers));
    dan_free_message(&(b->received));
}

