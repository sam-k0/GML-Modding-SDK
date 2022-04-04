// This populates the function table in the DLL
// Long way
show_debug_message(init_function_address("instance_create", function_get_closure("instance_create")))
show_debug_message(init_function_address("show_debug_message", function_get_closure("show_debug_message")))
show_debug_message(init_function_address("variable_global_set", function_get_closure("variable_global_set")))
show_debug_message(init_function_address("show_message", function_get_closure("show_message")))
// Short way
// instances
function_register("variable_instance_get")
function_register("instance_destroy")
// Ds lists
function_register("ds_list_create")
function_register("ds_list_destroy")
function_register("ds_list_clear")
function_register("ds_list_empty")
function_register("ds_list_size")
function_register("ds_list_set")
function_register("ds_list_add")


init_function_done(); // Sets a flag in the DLL
