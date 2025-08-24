# author: nakildias
# release_date: 24/08/2025
# last_updated: 24/08/2025
# requires_sudo: false
# version: 1.0.0
# size: 1KB

import os
import json
import sys

def get_safe_path(req_path, user_home):
    bin_dir = os.path.dirname(os.path.abspath(__file__))
    FILESYSTEM_ROOT = os.path.abspath(os.path.join(bin_dir, '..'))
    safe_base = FILESYSTEM_ROOT
    full_path = os.path.abspath(os.path.join(safe_base, req_path.lstrip('/\\')))
    if not full_path.startswith(safe_base):
        return None
    return full_path

def main():
    context_str = os.environ.get('WEBOS_CONTEXT')
    if not context_str:
        print("Error: Script must be run within WebOS.", file=sys.stderr)
        sys.exit(1)

    context = json.loads(context_str)
    cwd = context.get('cwd', '/')
    user_home = os.path.join('/home', context['user']['username'])

    current_path_server = get_safe_path(cwd, user_home)

    if current_path_server and os.path.isdir(current_path_server):
        try:
            print("\n".join(os.listdir(current_path_server)))
        except Exception as e:
            print(f"ls: error: {e}", file=sys.stderr)
            sys.exit(1)
    else:
        print("ls: cannot access '.': No such file or directory", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
