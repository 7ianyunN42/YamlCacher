
import yaml
from dist.yaml_cacher import get_yaml

def compare_yaml_cacher_to_python_lib(yml_path:str) -> bool:
     ref_dict = yaml.safe_load(open(yml_path, 'r',  encoding='utf-8'))
     sol_dict = get_yaml(yml_path)
     
     # recursively compare content in dictionary
     def compare_dicts(ref_dict, sol_dict):
         for key in ref_dict:
             if key not in sol_dict:
                 print(f"Key {key} not found in solution dictionary")
                 return False
             if isinstance(ref_dict[key], dict):
                 if not isinstance(sol_dict[key], dict):
                     print(f"Value of key {key} is not a dictionary in solution dictionary")
                     return False
                 if not compare_dicts(ref_dict[key], sol_dict[key]):
                     return False
             elif ref_dict[key] != sol_dict[key]:
                 print(f"Value of key {key} does not match in solution dictionary")
                 return False
         return True
     
     return compare_dicts(ref_dict, sol_dict)
            

def test_yaml_cacher():
    # iterate through all yml files in test folder
    import os
    for root, dirs, files in os.walk("test/inputs"):
        for file in files:
            if file.endswith(".yml") or file.endswith(".yaml"):
                yml_path = os.path.abspath(os.path.join(root, file))
                if compare_yaml_cacher_to_python_lib(yml_path):
                    pass
                    #print(f"{yml_path} - pass")
                else:
                    print(f"{yml_path} - fail")

def main():
    test_yaml_cacher()


if __name__ == "__main__":
    main()