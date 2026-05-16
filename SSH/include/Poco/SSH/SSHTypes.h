//
// SSHTypes.h
//
// Forward declarations of libssh opaque handles. Lets Poco::SSH public headers
// avoid <libssh/libssh.h>. Tags match upstream; safe to co-include with the
// real libssh headers.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHTypes_INCLUDED
#define SSH_SSHTypes_INCLUDED


struct ssh_session_struct;
struct ssh_channel_struct;
struct ssh_key_struct;
struct ssh_bind_struct;

using ssh_session = ssh_session_struct*;
using ssh_channel = ssh_channel_struct*;
using ssh_key     = ssh_key_struct*;
using ssh_bind    = ssh_bind_struct*;


#endif // SSH_SSHTypes_INCLUDED
