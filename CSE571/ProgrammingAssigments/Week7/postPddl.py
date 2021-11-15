import requests
import argparse
import json


def main(problem_number):
    data = {'domain': open(f'./q{str(problem_number)}_domain.pddl', 'r').read(),
            'problem': open(f'./q{str(problem_number)}_problem.pddl', 'r').read()}
    resp = requests.post('http://solver.planning.domains/solve', verify=False, json=data).json()
    print(json.dumps(resp, indent=4, sort_keys=True))
    with open(f'./output{str(problem_number)}', 'w') as f:
        f.write('\n'.join([act['name'] for act in resp['result']['plan']]))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Post PDDL file to the web service')
    parser.add_argument('--problem', type=int,
                        help='PDDL problem number to be posted')
    args = parser.parse_args()
    if not args.problem:
        raise Exception('No problem number provided')
    main(args.problem)
