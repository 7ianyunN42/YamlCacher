INPUTS_PARENT_FOLDER = "test/unit_tests"
PRINT_PASSED_TEST:bool = True

import yaml
from dist.yaml_cacher import get_yaml


def compare_yaml_cacher_to_python_lib(yml_path:str) -> bool:
    ref_dict = None
    try:
        ref_dict = yaml.safe_load(open(yml_path, 'r',  encoding='utf-8'))
    except Exception as e:
        return True
    
    sol_dict = get_yaml(yml_path)
     
    # recursively compare content in dictionary
    def compare_dicts(ref_dict, sol_dict):
        ret:bool = True
        for key in ref_dict:
            if key not in sol_dict:
                print(f"Key {key} not found in solution dictionary")
                ret = False
            if isinstance(ref_dict[key], dict):
                if not isinstance(sol_dict[key], dict):
                    print(f"type error for {key}: expected: {type(ref_dict[key])}, got: {type(sol_dict[key])}")
                    ret = False
                if not compare_dicts(ref_dict[key], sol_dict[key]):
                    return False
            elif ref_dict[key] != sol_dict[key]:
                print(f"value mismatch for {key}. expected: {ref_dict[key]} : {type(ref_dict[key])}, got: {sol_dict[key]} : {type(sol_dict[key])}")
                ret = False
        return ret
    
    return compare_dicts(ref_dict, sol_dict)
            

def test_yaml_cacher():
    # iterate through all yml files in test folder
    import os
    for root, dirs, files in os.walk(INPUTS_PARENT_FOLDER):
        for file in files:
            if file.endswith(".yml") or file.endswith(".yaml"):
                yml_path = os.path.abspath(os.path.join(root, file))
                if compare_yaml_cacher_to_python_lib(yml_path):
                    pass
                    print(f"{file} - pass") if PRINT_PASSED_TEST else None
                else:
                    pass
                    print(f"{file} - fail")

def main():
    test_yaml_cacher()
    


if __name__ == "__main__":
    main()