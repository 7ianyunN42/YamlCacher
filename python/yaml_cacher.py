from lib_yaml_cacher import get_yaml as get_yaml_impl

def get_yaml(absolute_path:str, keys:'list[str]'=None):
    """
    Load yml file. Return dictionary, list, or value.
    Parameters
    ----------
    keys: list[str] or None
        A series of keys used to traverse the yaml file. If None, return the entire file.
    """
    return get_yaml_impl(absolute_path, keys)