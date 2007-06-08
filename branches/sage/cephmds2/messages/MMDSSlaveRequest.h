// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */


#ifndef __MMDSSLAVEREQUEST_H
#define __MMDSSLAVEREQUEST_H

#include "msg/Message.h"
#include "mds/mdstypes.h"


class MMDSSlaveRequest : public Message {
 public:
  static const int OP_XLOCK = 1;
  static const int OP_XLOCKACK = -1;
  static const int OP_UNXLOCK = 3;
  static const int OP_PINDN = 4;
  static const int OP_UNPINDN = 5;
  static const int OP_FINISH = 6;

  const static char *get_opname(int o) {
	switch (o) { 
	case OP_XLOCK: return "xlock";
	case OP_XLOCKACK: return "xlock_ack";
	case OP_UNXLOCK: return "unxlock";
	case OP_PINDN: return "pin_dn";
	case OP_UNPINDN: return "unpin_dn";
	case OP_FINISH: return "finish";
	default: assert(0); return 0;
	}
  }

 private:
  metareqid_t reqid;
  char op;

  // for locking
  char lock_type;  // lock object type
  MDSCacheObjectInfo object_info;
  
  // for dn pins
  string dnpath;

 public:
  metareqid_t get_reqid() { return reqid; }
  int get_op() { return op; }
  bool is_reply() { return op < 0; }

  int get_lock_type() { return lock_type; }
  MDSCacheObjectInfo &get_object_info() { return object_info; }

  const string& get_dnpath() { return dnpath; }

  void set_lock_type(int t) { lock_type = t; }

  // ----
  MMDSSlaveRequest() : Message(MSG_MDS_SLAVE_REQUEST) { }
  MMDSSlaveRequest(metareqid_t ri, int o) : 
	Message(MSG_MDS_SLAVE_REQUEST),
	reqid(ri), op(o)
	{ }

  void encode_payload() {
	::_encode(reqid, payload);
	::_encode(op, payload);
    ::_encode(lock_type, payload);
	object_info._encode(payload);
    ::_encode(dnpath, payload);
  }
  void decode_payload() {
	int off = 0;
	::_decode(reqid, payload, off);
	::_decode(op, payload, off);
	::_decode(lock_type, payload, off);
	object_info._decode(payload, off);
    ::_decode(dnpath, payload, off);
  }

  char *get_type_name() { return "slave_request"; }
  void print(ostream& out) {
	out << "slave_request(" << reqid
		<< " " << get_opname(op) 
		<< ")";
  }  
	
};

#endif
