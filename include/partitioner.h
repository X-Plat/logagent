/***************************************************************************
 *
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * $Id: partitioner.h,v 1.1 2011/08/11 zhenpeng Exp $
 *
 **************************************************************************/

/**
 * @file    partitioner.h
 * @brief   pipelet拆分器
 * @author  zhenpeng(zhenpeng@baidu.com)
 * @version 1.0.0
 * @date    2011-08-11
 */

#ifndef __LOGAGENT_PARTITIONER_H
#define __LOGAGENT_PARTITIONER_H

#include <Configure.h>
#include <spreg.h>

#include "bigpipe_partitioner.h"

namespace bigpipe
{

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief 动态链接库中的切分函数原型
     *
     * @param[in] max_key    切分的范围
     * @param[in] line       原始消息
     * @param[in] len        消息长度
     * @param[in] extra_data 其他，未用到
     *
     * @return 成功返回[0, max_key)，< 0将会跳过这条消息
     */
    typedef int (*part_func_t)(unsigned int  max_key,
                               const char*   line,
                               unsigned int& len,
                               void*         extra_data);

    /**
     * @brief 动态链接库中的切分函数原型
     *
     * @param[in] in_buf       输入消息
     * @param[in] in_len       消息长度
     * @param[in] extra_data 其他
     * @param[out] tag_buf tag输出缓冲区
     * @param[out] tag_len tag输出缓冲区长度
     *
     * @return 成功返回>0(tag的长度), 失败返回-1
     */
    typedef int (*tag_func_t)(const char*   in_buf,
                               unsigned int in_len,
							   void *		extra_data,
							   char *tag_buf, unsigned int tag_len);

    typedef int (*extract_func_t)(const char *in_buf);

#ifdef __cplusplus
}
#endif

/**
 * @brief 日志切分类
 */
class partitioner_t: public bigpipe_partitioner_t
{
public:
    /**
     * @brief 构造函数
     */
    partitioner_t();

    /**
     * @brief 析构函数
     */
    ~partitioner_t();

    /**
     * @brief 初始化Hash
     *
     * @param[in] conf 加载完的Configure
     *
     * @return 成功返回0，出错返回非0
     */
    int init(comcfg::Configure &conf);

    /**
     * @brief 根据消息生成KEY
     *
     * @param msg 消息内容
     * @param len 消息长度
     * @param part_num 切分的个数
     *
     * @return [0, part_num)范围一个数
     */
    virtual int get_partition(const char *msg, unsigned int len, uint32_t part_num);

	/**
	 * @brief 根据消息生成KEY
	 *
	 * @param msg 消息内容
	 * @param len 消息长度
	 * @param tag_buf tag缓冲区地址
	 * @param tag_len tag缓冲区长度
	 *
	 * @note  当前为满足spider-arch ec应用，过滤出每台机器的日志
	 * @return 成功返回0，失败返回<0
	 */
	int get_tag(const char *msg, unsigned int len, char *tag_buf, int tag_len);

    /**
     * @brief 取得特定于消息的flag
     *
     * @param[in] msg
     * @param[in] len
     *
     * @return 成功返回flag,出错返回0xffffffff
     */
    uint64_t get_flag(const char *msg, size_t len);

    int64_t extract_field(const char *msg, unsigned int msg_len);
    
    int _get_field;

private:
    /**
     * @brief 
     *
     * @param partition_func_name
     *
     * @return 
     */
    bool get_last_partition_func(bsl::string& partition_func_name);

    /**
     * @brief 
     *
     * @param partition_func_name
     *
     * @return 
     */
    bool store_partition_func(bsl::string& partition_func_name);
private:
    bool        _is_init;               ///< 是否已初始化
    part_func_t _partition_func;        ///< 指向动态库的函数指针
    tag_func_t _tag_func;        ///< 指向动态库的函数指针
    bsl::string _partition_lib_path;    ///< 动态库路径
    bsl::string _partition_func_name;   ///< 动态库中的函数名称
    bsl::string _tag_func_name;   ///< 动态库中的函数名称
    bsl::string _flag_func_name;        ///< 提取flag的函数名称
    bsl::string _local_ip_str;        ///< 本地ip地址
    bsl::string _persist_path;
    bsl::string _pipe_name;

    //字段提取
    spreg_t *_field_spreg;
    spreg_match_t *_rmatch;
    extract_func_t _extract_func;
    bsl::string _extract_func_name;
    bsl::string _field_match_str;
    char _local_hostname[256];
    bsl::string _customize_tag;
};

} // end namespace

#endif
