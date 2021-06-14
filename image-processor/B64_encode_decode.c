#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//###################Encoding with base64######################

size_t b64_encoded_size(size_t inlen)
{
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char *b64_encode(const unsigned char *in, size_t len)
{
  
	unsigned char *out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (b64chars == NULL || len == 0)
		return NULL;

	elen = b64_encoded_size(len);
	out  = malloc(elen+1);
	out[elen] = '\0';

	for (i=0, j=0; i<len; i+=3, j+=4) {
		v = in[i];
		v = i+1 < len ? v << 8 | in[i+1] : v << 8;
		v = i+2 < len ? v << 8 | in[i+2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j+1] = b64chars[(v >> 12) & 0x3F];
		if (i+1 < len) {
			out[j+2] = b64chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < len) {
			out[j+3] = b64chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return out;
}

//############################# Decoding with Base 64 #####################################################
int b64invs[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51};


size_t b64_decoded_size(const char *in)
{
	size_t len;
	size_t ret;
	size_t i;

	if (in == NULL)
		return 0;

	len = strlen(in);
	ret = len / 4 * 3;

	for (i=len; i-->0; ) {
		if (in[i] == '=') {
			ret--;
		} else {
			break;
		}
	}

	return ret;
}

int b64_isvalidchar(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}


int b64_decode(const char *in, unsigned char *out, size_t outlen)
{
	size_t len;
	size_t i;
	size_t j;
	int    v;

	if (in == NULL || out == NULL)
		return 0;

	len = strlen(in);
	if (outlen < b64_decoded_size(in) || len % 4 != 0)
		return 0;

	for (i=0; i<len; i++) {
		if (!b64_isvalidchar(in[i])) {
			return 0;
		}
	}

	for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[in[i]-43];
		v = (v << 6) | b64invs[in[i+1]-43];
		v = in[i+2]=='=' ? v << 6 : (v << 6) | b64invs[in[i+2]-43];
		v = in[i+3]=='=' ? v << 6 : (v << 6) | b64invs[in[i+3]-43];

		out[j] = (v >> 16) & 0xFF;
		if (in[i+2] != '=')
			out[j+1] = (v >> 8) & 0xFF;
		if (in[i+3] != '=')
			out[j+2] = v & 0xFF;
	}

	return 1;
}


//#################################### MAIN #######################################

int picture_get(){//const struct _u_request * request, struct _u_response * response, void * user_data
    FILE *file;
    FILE *file_w;
    FILE *file_i;
	size_t b64_decode_len;
    size_t fileLen=0;
    char *buffer_decoded;
    char *base64_encode_string;
    char *base64_decode_string;
    unsigned char *buffer;
    const char * name = "/home/gabriel/Documentos/Repositorios/Servidor-Distribuido-Centralizado/image-processor/oggy.jpg";
    const char * name_file_encode= "/home/gabriel/Documentos/Repositorios/Servidor-Distribuido-Centralizado/image-processor/encode.txt";
    const char * name_file_decode= "/home/gabriel/Documentos/Repositorios/Servidor-Distribuido-Centralizado/image-processor/decode.txt";

    file = fopen(name , "rb");
    file_w = fopen(name_file_encode,"w");
    file_i = fopen(name_file_decode,"wb");
    if (!file)
    {
      fprintf(stderr, "Unable to open file %s", name);
      return -1;
    }

    //Get file length                                                                                                                                                                                          
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory                                                                                                                                                                                          
    buffer=(char *)malloc(fileLen+1);
    if (!buffer)
    {
      fprintf(stderr, "Memory error!");
      fclose(file);
      return -1;
    }

    //Read file contents into buffer  
    fread(buffer, fileLen, 1, file);
    //base64_encode_string = base64_encode(buffer, fileLen, output_length);
    base64_encode_string = b64_encode((const unsigned char *)buffer, fileLen);
    fprintf(file_w, "%s", base64_encode_string);
    b64_decode_len= b64_decoded_size(base64_encode_string)+1;
    //printf("%s\n",base64_encode_string);
    //base64_decode_string = (char *)malloc(b64_decode_len);
    //b64_decode(base64_encode_string, (unsigned char *)base64_decode_string, b64_decode_len);
    //base64_decode_string[b64_decode_len] = '\0';
    //fprintf(file_i,"%s",base64_decode_string);
    //printf("dec:     '%s'\n", base64_decode_string);
    //json_body=json_object ();
    //printf("%s\n",base64_encode_string);
    //printf("###############################################################################\n");
    //json_object_set_new(json_body,"b64_encoded",json_string(base64_encode_string));
    //ulfius_set_json_body_response(response, 200, json_body);
    //json_decref(json_body);
    free(buffer);
    fclose(file);
    fclose(file_w);
    return 0;
    //return U_CALLBACK_COMPLETE;
}

int main(){
    picture_get();
    system("base64 --decode /home/gabriel/Documentos/Repositorios/Servidor-Distribuido-Centralizado/image-processor/encode.txt > /home/gabriel/Documentos/Repositorios/Servidor-Distribuido-Centralizado/image-processor/decode.jpg");
}

//https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/
//https://linuxhint.com/bash_base64_encode_decode/