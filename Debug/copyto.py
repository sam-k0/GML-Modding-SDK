import shutil

GM_extension_path = "E:\\GameMaker Studio\\YAL-GameMaker function_get_address master function_get_address.gmx\\extensions\\function_get_address\\"
shutil.copy('function_get_address.dll', GM_extension_path)
shutil.copy('function_get_address.pdb', GM_extension_path)
print("Done")