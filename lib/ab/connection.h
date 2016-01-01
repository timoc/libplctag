/***************************************************************************
 *   Copyright (C) 2015 by OmanTek                                         *
 *   Author Kyle Hayes  kylehayes@omantek.com                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __AB_CONNECTION_H__
#define __AB_CONNECTION_H__ 1

#include <libplctag.h>
#include <libplctag_tag.h>
#include <platform.h>

typedef struct ab_session_t *ab_session_p; /* to get around circular .h includes */
typedef struct ab_connection_t *ab_connection_p;

struct ab_connection_t {
    ab_connection_p next;
    
    //int is_connected;
    //uint32_t targ_connection_id; /* the ID the target uses for this connection. */
    uint32_t orig_connection_id; /* the ID we use for this connection */
    //uint16_t packet;
    //uint16_t conn_serial_number;
    uint16_t conn_seq_num;

    /* need to save the connection path for later */
    //uint8_t conn_path[MAX_CONN_PATH];
    //uint8_t conn_path_size;
    ab_request_p fo_request;

    /* how do we talk to this device? */
    //int protocol_type;
    //int use_dhp_direct;
    //uint8_t dhp_src;
    //uint8_t dhp_dest;
    //uint16_t conn_params;

    //int connect_in_progress;
    char *path;
    ab_session_p session;
    int cpu_type;
    int status;
    int debug;
    int tag_count;
    int state;
    lock_t lock;
};


extern ab_connection_p connection_find_or_add(ab_session_p session, plc_tag tag);
extern void connection_inc_tag_count(ab_connection_p connection);
extern void connection_dec_tag_count(ab_connection_p connection);


#endif