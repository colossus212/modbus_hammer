#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <glib.h>
#include "test_suite.h"

HParser *modbus_read_coil_req_parser;
HParser *modbus_read_coil_resp_parser;

HParsedToken *act_modbus_coil_data(const HParseResult *p, void *user_data) {
  if (p->ast->token_type == TT_UINT)
    printf("Got coil data: %llu\n", p->ast->uint);
  return NULL;
}

HParsedToken *act_modbus_read_coil_req(const HParseResult *p, void *user_data) {
  printf("\n=== Coil read request:\n\tStarting address: %llu\n\tCoil Quantity: %llu\n", p->ast->seq->elements[1]->uint, p->ast->seq->elements[2]->uint);
  return NULL;
}
HParsedToken *act_modbus_read_coil_resp(const HParseResult *p, void *user_data) {
  // TODO something like act_modbus_read_coil_req, but iterate through elements and print values.
  return NULL;
}


void init_parser() {
  H_RULE(modbus_read_function_code, h_int_range(h_uint8(), 1, 1));

  H_RULE(modbus_address, h_uint16()); // No validation required

  H_RULE(modbus_coil_quantity, h_int_range(h_uint16(), 1, 2000)); // Must be between 1 and 2000

  H_ARULE(modbus_coil_data, h_bits(8, false));

  H_ARULE(modbus_read_coil_req, h_sequence(modbus_read_function_code, modbus_address, modbus_coil_quantity, h_end_p(), NULL));
  H_ARULE(modbus_read_coil_resp, h_sequence(modbus_read_function_code, h_length_value(h_uint8(), modbus_coil_data), h_end_p(), NULL));
  
  modbus_read_coil_req_parser = modbus_read_coil_req;
  modbus_read_coil_resp_parser = modbus_read_coil_resp;
}


static void test_modbus_read_coil_req() {
  g_check_parse_ok(modbus_read_coil_req_parser, PB_MIN, "\x01\x00\x01\x00\x01", 5);
}

static void test_modbus_read_coil_resp() {
  g_check_parse_ok(modbus_read_coil_resp_parser, PB_MIN, "\x01\x01\x01", 3); 
}


void register_modbus_tests() {
  g_test_add_func("/coil/read/req", test_modbus_read_coil_req);
  g_test_add_func("/coil/read/resp", test_modbus_read_coil_resp);
}

int main(int argc, char** argv) {
  init_parser();
  g_test_init(&argc, &argv, NULL);
  register_modbus_tests();
  g_test_run();
}

