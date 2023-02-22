//
//  H264ParseSPS.c
//
//  Created by XiangruiZhao on 2023/2/6.
//

#include "H264ParseSPS.h"
 
/**
 ��Ƶ��������Ϣ(Video usability information)����
 @param bs sps_bit_stream����
 @param info sps����֮�����Ϣ���ݼ��ṹ��
 @see E.1.1 VUI parameters syntax
 */
void vui_para_parse(sps_bit_stream *bs, sps_info_struct *info)
{
    UINT aspect_ratio_info_present_flag = u(bs, 1);
    if (aspect_ratio_info_present_flag) {
        UINT aspect_ratio_idc = u(bs, 8);
        if (aspect_ratio_idc == 255) {      //Extended_SAR
            u(bs, 16);      //sar_width
            u(bs, 16);      //sar_height
        }
    }
    
    UINT overscan_info_present_flag = u(bs, 1);
    if (overscan_info_present_flag) {
        u(bs, 1);       //overscan_appropriate_flag
    }
    
    UINT video_signal_type_present_flag = u(bs, 1);
    if (video_signal_type_present_flag) {
        u(bs, 3);       //video_format
        u(bs, 1);       //video_full_range_flag
        UINT colour_description_present_flag = u(bs, 1);
        if (colour_description_present_flag) {
            u(bs, 8);       //colour_primaries
            u(bs, 8);       //transfer_characteristics
            u(bs, 8);       //matrix_coefficients
        }
    }
    
    UINT chroma_loc_info_present_flag = u(bs, 1);
    if (chroma_loc_info_present_flag) {
        ue(bs);     //chroma_sample_loc_type_top_field
        ue(bs);     //chroma_sample_loc_type_bottom_field
    }
 
    UINT timing_info_present_flag = u(bs, 1);
    if (timing_info_present_flag) {
        UINT num_units_in_tick = u(bs, 32);
        UINT time_scale = u(bs, 32);
        UINT fixed_frame_rate_flag = u(bs, 1);
        
        info->fps = (UINT)((float)time_scale / (float)num_units_in_tick);
        if (fixed_frame_rate_flag) {
            info->fps = info->fps/2;
        }
    }
    
    UINT nal_hrd_parameters_present_flag = u(bs, 1);
    if (nal_hrd_parameters_present_flag) {
        //hrd_parameters()  //see E.1.2 HRD parameters syntax
    }
    
    //���������Ҫhrd_parameters()�����ӿ�ʵ�ֲ�����
    UINT vcl_hrd_parameters_present_flag = u(bs, 1);
    if (vcl_hrd_parameters_present_flag) {
        //hrd_parameters()
    }
    if (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag) {
        u(bs, 1);   //low_delay_hrd_flag
    }
    
    u(bs, 1);       //pic_struct_present_flag
    UINT bitstream_restriction_flag = u(bs, 1);
    if (bitstream_restriction_flag) {
        u(bs, 1);   //motion_vectors_over_pic_boundaries_flag
        ue(bs);     //max_bytes_per_pic_denom
        ue(bs);     //max_bits_per_mb_denom
        ue(bs);     //log2_max_mv_length_horizontal
        ue(bs);     //log2_max_mv_length_vertical
        ue(bs);     //max_num_reorder_frames
        ue(bs);     //max_dec_frame_buffering
    }
}
 
//See 7.3.1 NAL unit syntax
//See 7.3.2.1.1 Sequence parameter set data syntax
INT h264_parse_sps(const BYTE *data, UINT dataSize, sps_info_struct *info)
{
    if (!data || dataSize <= 0 || !info) return 0;
    INT ret = 0;
    
    BYTE *dataBuf = malloc(dataSize);
    memcpy(dataBuf, data, dataSize);        //���¿���һ�����ݣ���ֹ�Ƴ�������ʱ��ԭ�������Ӱ��
    del_emulation_prevention(dataBuf, &dataSize);
 
    sps_bit_stream bs = {0};
    sps_bs_init(&bs, dataBuf, dataSize);   //��ʼ��SPS�������ṹ��
    
    UINT forbidden_zero_bit = u(&bs, 1);      //forbidden_zero_bit
    UINT nal_ref_idc = u(&bs, 2);      //nal_ref_idc
    UINT nal_unit_type = u(&bs, 5);
 
    if (nal_unit_type == 0x7) {     //Nal SPS Flag
        info->profile_idc = u(&bs, 8);
        u(&bs, 1);      //constraint_set0_flag
        u(&bs, 1);      //constraint_set1_flag
        u(&bs, 1);      //constraint_set2_flag
        u(&bs, 1);      //constraint_set3_flag
        u(&bs, 1);      //constraint_set4_flag
        u(&bs, 1);      //constraint_set4_flag
        u(&bs, 2);      //reserved_zero_2bits
        info->level_idc = u(&bs, 8);
        
        ue(&bs);    //seq_parameter_set_id
        
        UINT chroma_format_idc = 1;     //�������ͼ�󲿷ָ�ʽ��4:2:0
        if (info->profile_idc == 100 || info->profile_idc == 110 || info->profile_idc == 122 ||
            info->profile_idc == 244 || info->profile_idc == 44 || info->profile_idc == 83 ||
            info->profile_idc == 86 || info->profile_idc == 118 || info->profile_idc == 128 ||
            info->profile_idc == 138 || info->profile_idc == 139 || info->profile_idc == 134 || info->profile_idc == 135) {
            chroma_format_idc = ue(&bs);
            if (chroma_format_idc == 3) {
                u(&bs, 1);      //separate_colour_plane_flag
            }
            
            ue(&bs);        //bit_depth_luma_minus8
            ue(&bs);        //bit_depth_chroma_minus8
            u(&bs, 1);      //qpprime_y_zero_transform_bypass_flag
            UINT seq_scaling_matrix_present_flag = u(&bs, 1);
            if (seq_scaling_matrix_present_flag) {
                UINT seq_scaling_list_present_flag[8] = {0};
                for (INT i=0; i<((chroma_format_idc != 3)?8:12); i++) {
                    seq_scaling_list_present_flag[i] = u(&bs, 1);
                    if (seq_scaling_list_present_flag[i]) {
                        if (i < 6) {    //scaling_list(ScalingList4x4[i], 16, UseDefaultScalingMatrix4x4Flag[i])
                        } else {    //scaling_list(ScalingList8x8[i ? 6], 64, UseDefaultScalingMatrix8x8Flag[i ? 6] )
                        }
                    }
                }
            }
        }
        
        ue(&bs);        //log2_max_frame_num_minus4
        UINT pic_order_cnt_type = ue(&bs);
        if (pic_order_cnt_type == 0) {
            ue(&bs);        //log2_max_pic_order_cnt_lsb_minus4
        } else if (pic_order_cnt_type == 1) {
            u(&bs, 1);      //delta_pic_order_always_zero_flag
            se(&bs);        //offset_for_non_ref_pic
            se(&bs);        //offset_for_top_to_bottom_field
            
            UINT num_ref_frames_in_pic_order_cnt_cycle = ue(&bs);
            INT *offset_for_ref_frame = (INT *)malloc((UINT)num_ref_frames_in_pic_order_cnt_cycle * sizeof(INT));
            for (INT i = 0; i<num_ref_frames_in_pic_order_cnt_cycle; i++) {
                offset_for_ref_frame[i] = se(&bs);
            }
            free(offset_for_ref_frame);
        }
        
        ue(&bs);      //max_num_ref_frames
        u(&bs, 1);      //gaps_in_frame_num_value_allowed_flag
        
        UINT pic_width_in_mbs_minus1 = ue(&bs);     //��36λ��ʼ
        UINT pic_height_in_map_units_minus1 = ue(&bs);      //47
        UINT frame_mbs_only_flag = u(&bs, 1);
        
        info->width = (INT)(pic_width_in_mbs_minus1 + 1) * 16;
        info->height = (INT)(2 - frame_mbs_only_flag) * (pic_height_in_map_units_minus1 + 1) * 16;
        
        if (!frame_mbs_only_flag) {
            u(&bs, 1);      //mb_adaptive_frame_field_flag
        }
        
        u(&bs, 1);     //direct_8x8_inference_flag
        UINT frame_cropping_flag = u(&bs, 1);
        if (frame_cropping_flag) {
            UINT frame_crop_left_offset = ue(&bs);
            UINT frame_crop_right_offset = ue(&bs);
            UINT frame_crop_top_offset = ue(&bs);
            UINT frame_crop_bottom_offset= ue(&bs);
            
            //See 6.2 Source, decoded, and output picture formats
            INT crop_unit_x = 1;
            INT crop_unit_y = 2 - frame_mbs_only_flag;      //monochrome or 4:4:4
            if (chroma_format_idc == 1) {   //4:2:0
                crop_unit_x = 2;
                crop_unit_y = 2 * (2 - frame_mbs_only_flag);
            } else if (chroma_format_idc == 2) {    //4:2:2
                crop_unit_x = 2;
                crop_unit_y = 2 - frame_mbs_only_flag;
            }
            
            info->width -= crop_unit_x * (frame_crop_left_offset + frame_crop_right_offset);
            info->height -= crop_unit_y * (frame_crop_top_offset + frame_crop_bottom_offset);
        }
        
        UINT vui_parameters_present_flag = u(&bs, 1);
        if (vui_parameters_present_flag) {
            vui_para_parse(&bs, info);
        }
     
        ret = 1;
    }
    free(dataBuf);
    
    return ret;
}
