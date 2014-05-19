/***************************************************************************
 *
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * $Id: partitioner.h,v 1.1 2011/08/11 zhenpeng Exp $
 *
 **************************************************************************/

/**
 * @file    partitioner.h
 * @brief   pipelet�����
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
     * @brief ��̬���ӿ��е��зֺ���ԭ��
     *
     * @param[in] max_key    �зֵķ�Χ
     * @param[in] line       ԭʼ��Ϣ
     * @param[in] len        ��Ϣ����
     * @param[in] extra_data ������δ�õ�
     *
     * @return �ɹ�����[0, max_key)��< 0��������������Ϣ
     */
    typedef int (*part_func_t)(unsigned int  max_key,
                               const char*   line,
                               unsigned int& len,
                               void*         extra_data);

    /**
     * @brief ��̬���ӿ��е��зֺ���ԭ��
     *
     * @param[in] in_buf       ������Ϣ
     * @param[in] in_len       ��Ϣ����
     * @param[in] extra_data ����
     * @param[out] tag_buf tag���������
     * @param[out] tag_len tag�������������
     *
     * @return �ɹ�����>0(tag�ĳ���), ʧ�ܷ���-1
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
 * @brief ��־�з���
 */
class partitioner_t: public bigpipe_partitioner_t
{
public:
    /**
     * @brief ���캯��
     */
    partitioner_t();

    /**
     * @brief ��������
     */
    ~partitioner_t();

    /**
     * @brief ��ʼ��Hash
     *
     * @param[in] conf �������Configure
     *
     * @return �ɹ�����0�������ط�0
     */
    int init(comcfg::Configure &conf);

    /**
     * @brief ������Ϣ����KEY
     *
     * @param msg ��Ϣ����
     * @param len ��Ϣ����
     * @param part_num �зֵĸ���
     *
     * @return [0, part_num)��Χһ����
     */
    virtual int get_partition(const char *msg, unsigned int len, uint32_t part_num);

	/**
	 * @brief ������Ϣ����KEY
	 *
	 * @param msg ��Ϣ����
	 * @param len ��Ϣ����
	 * @param tag_buf tag��������ַ
	 * @param tag_len tag����������
	 *
	 * @note  ��ǰΪ����spider-arch ecӦ�ã����˳�ÿ̨��������־
	 * @return �ɹ�����0��ʧ�ܷ���<0
	 */
	int get_tag(const char *msg, unsigned int len, char *tag_buf, int tag_len);

    /**
     * @brief ȡ���ض�����Ϣ��flag
     *
     * @param[in] msg
     * @param[in] len
     *
     * @return �ɹ�����flag,������0xffffffff
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
    bool        _is_init;               ///< �Ƿ��ѳ�ʼ��
    part_func_t _partition_func;        ///< ָ��̬��ĺ���ָ��
    tag_func_t _tag_func;        ///< ָ��̬��ĺ���ָ��
    bsl::string _partition_lib_path;    ///< ��̬��·��
    bsl::string _partition_func_name;   ///< ��̬���еĺ�������
    bsl::string _tag_func_name;   ///< ��̬���еĺ�������
    bsl::string _flag_func_name;        ///< ��ȡflag�ĺ�������
    bsl::string _local_ip_str;        ///< ����ip��ַ
    bsl::string _persist_path;
    bsl::string _pipe_name;

    //�ֶ���ȡ
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
