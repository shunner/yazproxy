/* $Id: proxy.h,v 1.15 2005-05-04 08:31:44 adam Exp $
   Copyright (c) 1998-2005, Index Data.

This file is part of the yaz-proxy.

YAZ proxy is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

YAZ proxy is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with YAZ proxy; see the file LICENSE.  If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
 */

#ifndef YAZ_PROXY_H_INCLUDED
#define YAZ_PROXY_H_INCLUDED

#include <yaz++/z-assoc.h>
#include <yaz++/z-query.h>
#include <yaz++/z-databases.h>
#include <yaz++/cql2rpn.h>
#include <yaz/cql.h>
#include <yazproxy/bw.h>

class Yaz_Proxy;

#define MAX_ZURL_PLEX 10

#define PROXY_LOG_APDU_CLIENT 1
#define PROXY_LOG_APDU_SERVER 2
#define PROXY_LOG_REQ_CLIENT 4
#define PROXY_LOG_REQ_SERVER 8

class Yaz_usemarcon;
class Yaz_ProxyConfig;
class Yaz_ProxyClient;
class Yaz_CharsetConverter;

enum YAZ_Proxy_MARCXML_mode {
    none,
    marcxml,
};

/// Information Retrieval Proxy Server.
class YAZ_EXPORT Yaz_Proxy : public Yaz_Z_Assoc {
 private:
    char *get_cookie(Z_OtherInformation **otherInfo);
    char *get_proxy(Z_OtherInformation **otherInfo);
    void get_charset_and_lang_negotiation(Z_OtherInformation **otherInfo,
	char **charstes, char **langs, int *selected);
    Yaz_ProxyClient *get_client(Z_APDU *apdu, const char *cookie,
				const char *proxy_host);
    void srw_get_client(const char *db, const char **backend_db);
    Z_APDU *result_set_optimize(Z_APDU *apdu);
    void shutdown();
    void releaseClient();    
    Yaz_ProxyClient *m_client;
    IYaz_PDU_Observable *m_PDU_Observable;
    Yaz_ProxyClient *m_clientPool;
    Yaz_Proxy *m_parent;
    int m_seqno;
    int m_max_clients;
    int m_log_mask;
    int m_keepalive_limit_bw;
    int m_keepalive_limit_pdu;
    int m_client_idletime;
    int m_target_idletime;
    int m_debug_mode;
    char *m_proxyTarget;
    char *m_default_target;
    char *m_proxy_negotiation_charset;
    char *m_proxy_negotiation_lang;
    long m_seed;
    char *m_optimize;
    int m_session_no;         // sequence for each client session
    char m_session_str[30];  // session string (time:session_no)
    Yaz_ProxyConfig *m_config;
    char *m_config_fname;
    int m_bytes_sent;
    int m_bytes_recv;
    int m_bw_max;
    Yaz_bw m_bw_stat;
    int m_pdu_max;
    Yaz_bw m_pdu_stat;
    Z_GDU *m_bw_hold_PDU;
    int m_max_record_retrieve;
    void handle_max_record_retrieve(Z_APDU *apdu);
    int handle_authentication(Z_APDU *apdu);
    void display_diagrecs(Z_DiagRec **pp, int num);
    Z_Records *create_nonSurrogateDiagnostics(ODR o, int error,
					      const char *addinfo);

    Z_APDU *handle_query_validation(Z_APDU *apdu);
    Z_APDU *handle_query_transformation(Z_APDU *apdu);
    Z_APDU *handle_query_charset_conversion(Z_APDU *apdu);

    Z_APDU *handle_syntax_validation(Z_APDU *apdu);

    void handle_charset_lang_negotiation(Z_APDU *apdu);

    const char *load_balance(const char **url);
    int m_reconfig_flag;
    Yaz_ProxyConfig *check_reconfigure();
    int m_request_no;
    int m_invalid_session;
    YAZ_Proxy_MARCXML_mode m_marcxml_mode;
    void *m_stylesheet_xsp;  // Really libxslt's xsltStylesheetPtr 
    int m_stylesheet_offset;
    Z_APDU *m_stylesheet_apdu;
    Z_NamePlusRecordList *m_stylesheet_nprl;
    char *m_schema;
    char *m_backend_type;
    char *m_backend_charset;
    int m_frontend_type;
    void convert_to_frontend_type(Z_NamePlusRecordList *p);
    void convert_to_marcxml(Z_NamePlusRecordList *p, const char *charset);
    int convert_xsl(Z_NamePlusRecordList *p, Z_APDU *apdu);
    void convert_xsl_delay();
    Z_APDU *m_initRequest_apdu;
    int m_initRequest_preferredMessageSize;
    int m_initRequest_maximumRecordSize;
    Z_Options *m_initRequest_options;
    Z_ProtocolVersion *m_initRequest_version;
    char **m_initRequest_oi_negotiation_charsets;
    int m_initRequest_oi_negotiation_num_charsets;
    char **m_initRequest_oi_negotiation_langs;
    int m_initRequest_oi_negotiation_num_langs;
    int m_initRequest_oi_negotiation_selected;
    NMEM m_initRequest_mem;
    Z_APDU *m_apdu_invalid_session;
    NMEM m_mem_invalid_session;
    int send_PDU_convert(Z_APDU *apdu);
    ODR m_s2z_odr_init;
    ODR m_s2z_odr_search;
    int m_s2z_hit_count;
    int m_s2z_packing;
    char *m_s2z_database;
    Z_APDU *m_s2z_init_apdu;
    Z_APDU *m_s2z_search_apdu;
    Z_APDU *m_s2z_present_apdu;
    char *m_s2z_stylesheet;
    char *m_soap_ns;
    int file_access(Z_HTTP_Request *hreq);
    int send_to_srw_client_error(int error, const char *add);
    int send_to_srw_client_ok(int hits, Z_Records *records, int start);
    int send_http_response(int code);
    int send_srw_response(Z_SRW_PDU *srw_pdu);
    int send_srw_explain_response(Z_SRW_diagnostic *diagnostics,
				  int num_diagnostics);
    int z_to_srw_diag(ODR o, Z_SRW_searchRetrieveResponse *srw_res,
		      Z_DefaultDiagFormat *ddf);
    int m_http_keepalive;
    const char *m_http_version;
    Yaz_cql2rpn m_cql2rpn;
    void *m_time_tv;
    void logtime();
    Z_ElementSetNames *mk_esn_from_schema(ODR o, const char *schema);
    Z_ReferenceId *m_referenceId;
    NMEM m_referenceId_mem;
#define NO_SPARE_SOLARIS_FD 10
    int m_lo_fd[NO_SPARE_SOLARIS_FD];
    void low_socket_open();
    void low_socket_close();
    char *m_usemarcon_ini_stage1;
    char *m_usemarcon_ini_stage2;
    Yaz_usemarcon *m_usemarcon;
    Yaz_CharsetConverter *m_charset_converter;
 public:
    Yaz_Proxy(IYaz_PDU_Observable *the_PDU_Observable,
	      Yaz_Proxy *parent = 0);
    ~Yaz_Proxy();
    void inc_request_no();
    void recv_GDU(Z_GDU *apdu, int len);
    void handle_incoming_HTTP(Z_HTTP_Request *req);
    void handle_incoming_Z_PDU(Z_APDU *apdu);
    IYaz_PDU_Observer* sessionNotify
	(IYaz_PDU_Observable *the_PDU_Observable, int fd);
    void failNotify();
    void timeoutNotify();
    void connectNotify();
    void markInvalid();
    const char *option(const char *name, const char *value);
    void set_default_target(const char *target);
    void set_proxy_negotiation (const char *charset, const char *lang);
    void set_query_charset(const char *charset);
    char *get_proxy_target() { return m_proxyTarget; };
    char *get_session_str() { return m_session_str; };
    void set_max_clients(int m) { m_max_clients = m; };
    void set_client_idletime (int t) { m_client_idletime = (t > 1) ? t : 600; };
    void set_target_idletime (int t) { m_target_idletime = (t > 1) ? t : 600; };
    int get_target_idletime () { return m_target_idletime; }
    int set_config(const char *name);
    void reconfig() { m_reconfig_flag = 1; }
    int send_to_client(Z_APDU *apdu);
    int server(const char *addr);
    void pre_init();
    int get_log_mask() { return m_log_mask; };
    int handle_init_response_for_invalid_session(Z_APDU *apdu);
    void set_debug_mode(int mode);
};

#endif
