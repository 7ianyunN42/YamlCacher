
import time
import yaml
from yaml_cacher import get_yaml

YML_PATH:str = '/home/n42/repo/YamlCacher/test/test.yml'


def load_yml_old(full_path=None, numpy_evaluate=False):
    """
    Load yml file. Return dictionary.
    Parameters
    ----------
    full_path: str
        YAML file path.
    numpy_evaluate: bool
        Whether or not to parse for strings with '.np' and evaluate.

    Returns
    -------
    settings: dict
        Dictionary of data loaded.
    """
    if full_path is None:
        return

    settings = {}
    try:
        with open(full_path, 'r',  encoding='utf-8') as f:
            settings = yaml.safe_load(f)

        if numpy_evaluate is True:
            settings = load_yml_ref_evaluate(settings)

    except yaml.YAMLError as _:
        print("Error loading YAML file: {}".format(full_path))

    return settings

def load_yml_ref_evaluate(settings):
    """loads reference variables, updates ocurrances with values and evaluates numpy expressions

    Parameters
    ----------
    settings : dict
        dictionary which needs to be evaluated

    Returns
    -------
    dict
        dictionary with resolved numpy expressions

    """
    return settings # skipping everything for now
    from pulse_sequence.kspace_trajectories.kspace_trajectories_utils import evaluate_functions
    # reference evaluate #GA Recipe top params
    settings, reference_dict = extract_dictionary_references(settings, {}, copy.deepcopy(settings))

    # evaluate reference variables
    if len(reference_dict) != 0:
        reference_dict = numpy_update(reference_dict, {})
    settings = numpy_update(settings, reference_dict)

    settings = evaluate_functions(settings, reference_dict)

    return settings


REP = 10
def benchMarkNewLib():
    start_time = time.time()
    for i in range(REP):
        get_yaml(YML_PATH)
    end_time = time.time()
    total_time = end_time - start_time
    print(f"Total time taken for {REP} calls: {total_time} seconds")
    
def benchMarkOldLib():
    start_time = time.time()
    for i in range(REP):
        load_yml_old(YML_PATH)
    end_time = time.time()
    total_time = end_time - start_time
    print(f"Total time taken for {REP} calls: {total_time} seconds")
    

def benchMark():
    benchMarkNewLib()
    benchMarkOldLib()
if __name__ == "__main__":
    benchMark()