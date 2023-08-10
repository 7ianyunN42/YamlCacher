from lib_yaml_cacher import get_yaml as get_yaml_impl

def get_yaml(absolute_path:str, keys:'list[str]'=None):
    return get_yaml_impl(absolute_path, keys)