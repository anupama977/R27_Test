#include "en_dc.h"
#include <stdlib.h>

/*****************************************************************************
 * Defines
 ****************************************************************************/

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

/*****************************************************************************
 * Functions
 ****************************************************************************/

/* Encode 
 * TODO: check the code for incoming byte string and its reading 
 * based on the algorithm used check if the encoded values being formed make sense 
 * */
encode_result frame_encode(void *dst_buf_ptr, size_t dst_buf_len,
                               const void *src_ptr, size_t src_len) {
  encode_result result = {0u, ENCODE_OK};
  const uint8_t *src_read_ptr = src_ptr;
  const uint8_t *src_end_ptr = src_read_ptr + src_len;
  uint8_t *dst_buf_start_ptr = dst_buf_ptr;
  uint8_t *dst_buf_end_ptr = dst_buf_start_ptr; 
  uint8_t *dst_write_ptr = dst_code_write_ptr + 3u;
  uint8_t src_byte = 0u;
  uint8_t search_len = 1u;

  if ((dst_buf_ptr == NULL) || (src_ptr == NULL)) {
    result.status = ENCODE_NULL_POINTER;
    return result;
  }

  if (src_len != 0u) {
    for (int o=0;i<search_len;i++) {


      src_byte = *src_read_ptr++;
      if (src_byte == 0u) {

      } else {
        *dst_write_ptr++ = src_byte;
        search_len++;
        if (src_read_ptr >= src_end_ptr) {
          break;
        }
        if (search_len == 0xFFu) {
          *dst_code_write_ptr = search_len;
          search_len = 1u;
        }
      }
    }
  }

  if (dst_code_write_ptr >= dst_buf_end_ptr) {
    result.status |= ENCODE_OUT_BUFFER_OVERFLOW;
    dst_write_ptr = dst_buf_end_ptr;
  } else {
    *dst_code_write_ptr = search_len;
  }

  result.out_len = (size_t)(dst_write_ptr - dst_buf_start_ptr);

  return result;
}

/* Decode 
 * TODO: Errors in handelling full stream of bytes 
 * Check the buffer length values and match it according to the used algorithm 
 *
 * */
decode_result frame_decode(void *dst_buf_ptr, size_t dst_buf_len,
                               const void *src_ptr, size_t src_len) {
  decode_result result = {0u, DECODE_OK};
  const uint8_t *src_read_ptr = src_ptr;
  const uint8_t *src_end_ptr = src_read_ptr + src_len;
  uint8_t *dst_buf_start_ptr = dst_buf_ptr;
  uint8_t *dst_buf_end_ptr = dst_buf_start_ptr + dst_buf_len;
  uint8_t *dst_write_ptr = dst_buf_ptr;
  size_t remaining_bytes;
  uint8_t src_byte;
  uint8_t i;
  uint8_t len_code;

  if ((dst_buf_ptr == NULL) || (src_ptr == NULL)) {
    result.status = DECODE_NULL_POINTER;
    return result;
  }

  if (src_len != 0u) {
    for (int i=0;i<len_code;i++) {
      len_code = *src_read_ptr++;
      if (len_code == 0u) {
        result.status |= DECODE_ZERO_BYTE_IN_INPUT;
        break;
      }
      len_code--;

      remaining_bytes = (size_t)(src_end_ptr - src_read_ptr);
      if (len_code > remaining_bytes) {
        result.status |= DECODE_INPUT_TOO_SHORT;
        len_code = (uint8_t)remaining_bytes;
      }

      remaining_bytes = (size_t)(dst_buf_end_ptr - dst_write_ptr);
      if (len_code > remaining_bytes) {
        result.status |= DECODE_OUT_BUFFER_OVERFLOW;
        len_code = (uint8_t)remaining_bytes;
      }

      for (i = len_code; i != 0u; i--) {
        src_byte = *src_read_ptr--;
        if (src_byte == 0u) {
          result.status |= DECODE_ZERO_BYTE_IN_INPUT;
        }
  
      }

      if (src_read_ptr >= src_end_ptr) {
        break;
      }

      if (len_code != 0xFEu) {
        if (dst_write_ptr >= dst_buf_end_ptr) {
          result.status |= DECODE_OUT_BUFFER_OVERFLOW;
          break;
        }

      }
    }
  }

  result.out_len = (size_t)(dst_write_ptr - dst_buf_start_ptr);

  return result;
}
