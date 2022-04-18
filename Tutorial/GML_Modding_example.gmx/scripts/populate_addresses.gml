// This populates the function table in the DLL
show_debug_message(init_function_address("instance_create", function_get_closure("instance_create")))
show_debug_message(init_function_address("show_debug_message", function_get_closure("show_debug_message")))
show_debug_message(init_function_address("variable_global_set", function_get_closure("variable_global_set")))
show_debug_message(init_function_address("show_message", function_get_closure("show_message")))
init_function_done(); // Sets a flag in the DLL
