//
// Created by john on 2020/5/26.
//

#include "SSDB_impl.h"

#include "util/strings.h"
#include <signal.h>

namespace ssdb{

    inline static
    Status _read_list(const std::vector<std::string> *resp, std::vector<std::string> *ret){
        Status s(resp);
        if(s.ok()){
            std::vector<std::string>::const_iterator it;
            for(it = resp->begin() + 1; it != resp->end(); it++){
                ret->push_back(*it);
            }
        }
        return s;
    }

    inline static
    Status _read_int64(const std::vector<std::string> *resp, int64_t *ret){
        Status s(resp);
        if(s.ok()){
            if(resp->size() >= 2){
                if(ret){
                    *ret = str_to_int64(resp->at(1));
                }
            }else{
                return Status("server_error");
            }
        }
        return s;
    }

    inline static
    Status _read_str(const std::vector<std::string> *resp, std::string *ret){
        Status s(resp);
        if(s.ok()){
            if(resp->size() >= 2){
                *ret = resp->at(1);
            }else{
                return Status("server_error");
            }
        }
        return s;
    }

    ClientImpl::ClientImpl(){
        link = NULL;
    }

    ClientImpl::~ClientImpl(){
        if(link){
            delete link;
        }
    }

    Client* Client::connect(const char *ip, int port){
        return Client::connect(std::string(ip), port);
    }

    Client* Client::connect(const std::string &ip, int port){
        static bool inited = false;
        if(!inited){
            inited = true;
            signal(SIGPIPE, SIG_IGN);
        }
        ClientImpl *client = new ClientImpl();
        client->link = Link::connect(ip.c_str(), port);
        if(client->link == NULL){
            delete client;
            return NULL;
        }
        return client;
    }

    const std::vector<std::string>* ClientImpl::request(const std::vector<std::string> &req){
        if(link->send(req) == -1){
            return NULL;
        }
        if(link->flush() == -1){
            return NULL;
        }
        const std::vector<Bytes> *packet = link->response();
        if(packet == NULL){
            return NULL;
        }
        resp_.clear();
        for(std::vector<Bytes>::const_iterator it=packet->begin(); it!=packet->end(); it++){
            const Bytes &b = *it;
            resp_.push_back(b.String());
        }
        return &resp_;
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd){
        std::vector<std::string> req;
        req.push_back(cmd);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        req.push_back(s3);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        req.push_back(s3);
        req.push_back(s4);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        req.push_back(s3);
        req.push_back(s4);
        req.push_back(s5);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5, const std::string &s6){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        req.push_back(s3);
        req.push_back(s4);
        req.push_back(s5);
        req.push_back(s6);
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::vector<std::string> &s2){
        std::vector<std::string> req;
        req.push_back(cmd);
        for(std::vector<std::string>::const_iterator it = s2.begin(); it != s2.end(); ++it){
            req.push_back(*it);
        }
        return request(req);
    }

    const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::vector<std::string> &s3){
        std::vector<std::string> req;
        req.push_back(cmd);
        req.push_back(s2);
        for(std::vector<std::string>::const_iterator it = s3.begin(); it != s3.end(); ++it){
            req.push_back(*it);
        }
        return request(req);
    }

/******************** misc *************************/

    Status ClientImpl::dbsize(int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("dbsize");
        return _read_int64(resp, ret);
    }

    Status ClientImpl::get_kv_range(std::string *start, std::string *end){
        const std::vector<std::string> *resp;
        resp = this->request("get_kv_range");
        Status s(resp);
        if(s.ok()){
            *start = resp->at(1);
            *end = resp->at(2);
        }
        return s;
    }

    Status ClientImpl::set_kv_range(const std::string &start, const std::string &end){
        const std::vector<std::string> *resp;
        resp = this->request("set_kv_range", start, end);
        Status s(resp);
        return s;
    }

/******************** KV *************************/

    Status ClientImpl::get(const std::string &key, std::string *val){
        const std::vector<std::string> *resp;
        resp = this->request("get", key);
        return _read_str(resp, val);
    }

    Status ClientImpl::set(const std::string &key, const std::string &val){
        const std::vector<std::string> *resp;
        resp = this->request("set", key, val);
        Status s(resp);
        return s;
    }

    Status ClientImpl::setx(const std::string &key, const std::string &val, int ttl){
        const std::vector<std::string> *resp;
        resp = this->request("setx", key, val, str(ttl));
        Status s(resp);
        return s;
    }

    Status ClientImpl::del(const std::string &key){
        const std::vector<std::string> *resp;
        resp = this->request("del", key);
        Status s(resp);
        return s;
    }

    Status ClientImpl::incr(const std::string &key, int64_t incrby, int64_t *ret){
        std::string s_incrby = str(incrby);
        const std::vector<std::string> *resp;
        resp = this->request("incr", key, s_incrby);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::keys(const std::string &key_start, const std::string &key_end,
                            uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("keys", key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::scan(const std::string &key_start, const std::string &key_end,
                            uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("scan", key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }


    //nlist

    Status ClientImpl::nlist(const std::string &key_start, const std::string &key_end,
                            uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("nlist", key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }


    //nscan
    Status ClientImpl::nscan(const std::string &key, int64_t start ,int64_t end ,uint64_t offset,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("nscan", key, str(start), str(end),str(offset), s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::ncmget(const std::string &key, uint64_t number ,  std::vector<std::string> *ret)
    {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("ncmget", key, s_number);
        return _read_list(resp, ret);
    }



    Status ClientImpl::rscan(const std::string &key_start, const std::string &key_end,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("rscan", key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_get(const std::vector<std::string> &keys, std::vector<std::string> *ret){
        const std::vector<std::string> *resp;
        resp = this->request("multi_get", keys);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_set(const std::map<std::string, std::string> &kvs){
        const std::vector<std::string> *resp;
        std::vector<std::string> list;
        for(std::map<std::string, std::string>::const_iterator it = kvs.begin();
            it != kvs.end(); ++it)
        {
            list.push_back(it->first);
            list.push_back(it->second);
        }
        resp = this->request("multi_set", list);
        Status s(resp);
        return s;
    }

    Status ClientImpl::multi_del(const std::vector<std::string> &keys){
        const std::vector<std::string> *resp;
        resp = this->request("multi_del", keys);
        Status s(resp);
        return s;
    }


/******************** hash *************************/


    Status ClientImpl::hget(const std::string &name, const std::string &key, std::string *val){
        const std::vector<std::string> *resp;
        resp = this->request("hget", name, key);
        return _read_str(resp, val);
    }

    Status ClientImpl::hset(const std::string &name, const std::string &key, const std::string &val){
        const std::vector<std::string> *resp;
        resp = this->request("hset", name, key, val);
        Status s(resp);
        return s;
    }

    Status ClientImpl::hdel(const std::string &name, const std::string &key){
        const std::vector<std::string> *resp;
        resp = this->request("hdel", name, key);
        Status s(resp);
        return s;
    }

    Status ClientImpl::hincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
        std::string s_incrby = str(incrby);
        const std::vector<std::string> *resp;
        resp = this->request("hincr", name, key, s_incrby);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::hsize(const std::string &name, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("hsize", name);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::hclear(const std::string &name, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("hclear", name);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::hkeys(const std::string &name,
                             const std::string &key_start, const std::string &key_end,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("hkeys", name, key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::hscan(const std::string &name,
                             const std::string &key_start, const std::string &key_end,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("hscan", name, key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::hrscan(const std::string &name,
                              const std::string &key_start, const std::string &key_end,
                              uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("hrscan", name, key_start, key_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_hget(const std::string &name, const std::vector<std::string> &keys,
                                  std::vector<std::string> *ret){
        const std::vector<std::string> *resp;
        resp = this->request("multi_hget", name, keys);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_hset(const std::string &name, const std::map<std::string, std::string> &kvs){
        const std::vector<std::string> *resp;
        std::vector<std::string> list;
        for(std::map<std::string, std::string>::const_iterator it = kvs.begin();
            it != kvs.end(); ++it)
        {
            list.push_back(it->first);
            list.push_back(it->second);
        }
        resp = this->request("multi_hset", name, list);
        Status s(resp);
        return s;
    }

    Status ClientImpl::multi_hdel(const std::string &name, const std::vector<std::string> &keys){
        const std::vector<std::string> *resp;
        resp = this->request("multi_hdel", name, keys);
        Status s(resp);
        return s;
    }

/******************** zset *************************/



    Status ClientImpl::zget(const std::string &name, const std::string &key, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("zget", name, key);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zset(const std::string &name, const std::string &key, int64_t score){
        std::string s_score = str(score);
        const std::vector<std::string> *resp;
        resp = this->request("zset", name, key, s_score);
        Status s(resp);
        return s;
    }

    Status ClientImpl::zdel(const std::string &name, const std::string &key){
        const std::vector<std::string> *resp;
        resp = this->request("zdel", name, key);
        Status s(resp);
        return s;
    }

    Status ClientImpl::zincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
        std::string s_incrby = str(incrby);
        const std::vector<std::string> *resp;
        resp = this->request("zincr", name, key, s_incrby);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zsize(const std::string &name, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("zsize", name);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zclear(const std::string &name, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("zclear", name);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zrank(const std::string &name, const std::string &key, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("zrank", name, key);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zrrank(const std::string &name, const std::string &key, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("zrrank", name, key);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::zrange(const std::string &name,
                              uint64_t offset, uint64_t limit,
                              std::vector<std::string> *ret)
    {
        std::string s_offset = str(offset);
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("zrange", name, s_offset, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::zrrange(const std::string &name,
                               uint64_t offset, uint64_t limit,
                               std::vector<std::string> *ret)
    {
        std::string s_offset = str(offset);
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("zrrange", name, s_offset, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::zkeys(const std::string &name, const std::string &key_start,
                             int64_t *score_start, int64_t *score_end,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_score_start = score_start? str(*score_start) : "";
        std::string s_score_end = score_end? str(*score_end) : "";
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("zkeys", name, key_start, s_score_start, s_score_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::zscan(const std::string &name, const std::string &key_start,
                             int64_t *score_start, int64_t *score_end,
                             uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_score_start = score_start? str(*score_start) : "";
        std::string s_score_end = score_end? str(*score_end) : "";
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("zscan", name, key_start, s_score_start, s_score_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::zrscan(const std::string &name, const std::string &key_start,
                              int64_t *score_start, int64_t *score_end,
                              uint64_t limit, std::vector<std::string> *ret)
    {
        std::string s_score_start = score_start? str(*score_start) : "";
        std::string s_score_end = score_end? str(*score_end) : "";
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("zrscan", name, key_start, s_score_start, s_score_end, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_zget(const std::string &name, const std::vector<std::string> &keys,
                                  std::vector<std::string> *ret){
        const std::vector<std::string> *resp;
        resp = this->request("multi_zget", name, keys);
        return _read_list(resp, ret);
    }

    Status ClientImpl::multi_zset(const std::string &name, const std::map<std::string, int64_t> &kss){
        const std::vector<std::string> *resp;
        std::vector<std::string> s_kss;
        for(std::map<std::string, int64_t>::const_iterator it = kss.begin();
            it != kss.end(); ++it)
        {
            s_kss.push_back(it->first);
            s_kss.push_back(str(it->second));
        }
        resp = this->request("multi_zset", name, s_kss);
        Status s(resp);
        return s;
    }

    Status ClientImpl::multi_zdel(const std::string &name, const std::vector<std::string> &keys){
        const std::vector<std::string> *resp;
        resp = this->request("multi_zdel", name, keys);
        Status s(resp);
        return s;
    }

    Status ClientImpl::qpush(const std::string &name, const std::string &item, int64_t *ret_size){
        const std::vector<std::string> *resp;
        resp = this->request("qpush", name, item);
        Status s(resp);
        if(ret_size != NULL && s.ok()){
            if(resp->size() > 1){
                *ret_size = str_to_int64(resp->at(1));
            }else{
                return Status("error");
            }
        }
        return s;
    }

    Status ClientImpl::qpush(const std::string &name, const std::vector<std::string> &items, int64_t *ret_size){
        const std::vector<std::string> *resp;
        resp = this->request("qpush", name, items);
        Status s(resp);
        if(ret_size != NULL && s.ok()){
            if(resp->size() > 1){
                *ret_size = str_to_int64(resp->at(1));
            }else{
                return Status("error");
            }
        }
        return s;
    }

    Status ClientImpl::qpop(const std::string &name, std::string *item){
        const std::vector<std::string> *resp;
        resp = this->request("qpop", name);
        return _read_str(resp, item);
    }

    Status ClientImpl::qpop(const std::string &name, int64_t limit, std::vector<std::string> *ret){
        const std::vector<std::string> *resp;
        resp = this->request("qpop", name, str(limit));
        return _read_list(resp, ret);
    }

    Status ClientImpl::qslice(const std::string &name,
                              int64_t begin, int64_t end,
                              std::vector<std::string> *ret)
    {
        std::string s_begin = str(begin);
        std::string s_end = str(end);
        const std::vector<std::string> *resp;
        resp = this->request("qslice", name, s_begin, s_end);
        return _read_list(resp, ret);
    }

    Status ClientImpl::qrange(const std::string &name, int64_t begin, int64_t limit, std::vector<std::string> *ret){
        std::string s_begin = str(begin);
        std::string s_limit = str(limit);
        const std::vector<std::string> *resp;
        resp = this->request("qrange", name, s_begin, s_limit);
        return _read_list(resp, ret);
    }

    Status ClientImpl::qclear(const std::string &name, int64_t *ret){
        const std::vector<std::string> *resp;
        resp = this->request("qclear", name);
        return _read_int64(resp, ret);
    }

    Status ClientImpl::nset(const std::string &name, int64_t number, std::string &value) {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("nset", name, s_number, value);
        Status s(resp);
        return s;
    }
    Status ClientImpl::nset(const std::string &name, int64_t number, std::string &value, int ttl) {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("nset", name, s_number, value, str(ttl));
        Status s(resp);
        return s;
    }
    Status ClientImpl::nget(const std::string &name, int64_t number, std::string *value) {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("nget", name, s_number);
        return _read_str(resp, value);
    }
    Status ClientImpl::ndel(const std::string &name, int64_t number) {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("ndel", name, s_number);
        Status s(resp);
        return s;
    }
    Status ClientImpl::nexists(const std::string &name, int64_t number) {
        std::string s_number = str(number);
        const std::vector<std::string> *resp;
        resp = this->request("nexists", name, s_number);
        Status s(resp);
        return s;
    }
//Status ClientImpl::nmset(const std::string &name, const std::vector<int64_t, std::string> &elements);
//Status ClientImpl::nmdel(const std::string &name, const std::vector<int64_t> &numbers);
//Status ClientImpl::nrange(const std::string &name, int64_t offset, int64_t limit, std::vector<std::string> *values);
//Status ClientImpl::nrrange(const std::string &name, int64_t offset, int64_t limit, std::vector<std::string> *values);
//Status ClientImpl::nscan(const std::string &name, int64_t nstart, int64_t nend, int64_t offset, int64_t limit, std::vector<int64_t, std::string> *elements);
//Status ClientImpl::nrscan(const std::string &name, int64_t nstart, int64_t nend, int64_t offset, int64_t limit, std::vector<int64_t, std::string> *elements);
//Status ClientImpl::nsize(const std::string &name, int64_t *size);
//Status ClientImpl::nclear(const std::string &name);
//Status ClientImpl::nsum(const std::string &name, int64_t nstart, int64_t nend, int64_t *ret);
//Status ClientImpl::nava(const std::string &name, int64_t nstart, int64_t nend, int64_t *ret);
//Status ClientImpl::nremrangebyscore(const std::string &name, int64_t nstart, int64_t nend);
//Status ClientImpl::nremrangebyrank(const std::string &name, int64_t nstart, int64_t nend);
//Status ClientImpl::npop_front(const std::string &name, int64_t limit, std::vector<std::string> *values);
//Status ClientImpl::npop_back(const std::string &name, int64_t limit, std::vector<std::string> *values);
//Status ClientImpl::nexpire(const std::string &name, int64_t ttl);
//Status ClientImpl::nttl(const std::string &name, int64_t *ret);
//Status ClientImpl::ns_expire(const std::string &name, int64_t number, int64_t ttl);
//Status ClientImpl::ns_ttl(const std::string &name,int64_t number, int64_t *ret);

}; // namespace ssdb
