# author: nakildias
# release_date: 24/08/2025
# last_updated: 24/08/2025
# requires_sudo: false
# version: 1.0.0
# size: 1KB

import os
import json
import sys
import shutil

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
    args = context.get('args', [])
    cwd = context.get('cwd', '/')
    user_home = os.path.join('/home', context['user']['username'])

    if len(args) < 2:
        print("cp: missing destination file operand after source", file=sys.stderr)
        sys.exit(1)

    sources = args[:-1]
    dest = args[-1]
    dest_path_server = get_safe_path(os.path.join(cwd, dest), user_home)

    if not dest_path_server:
        print(f"cp: destination directory does not exist or is inaccessible", file=sys.stderr)
        sys.exit(1)

    for source in sources:
        source_path_server = get_safe_path(os.path.join(cwd, source), user_home)
        if not source_path_server or not os.path.exists(source_path_server):
            print(f"cp: cannot stat '{source}': No such file or directory", file=sys.stderr)
            continue

        try:
            if os.path.isdir(source_path_server):
                shutil.copytree(source_path_server, os.path.join(dest_path_server, os.path.basename(source)))
            else:
                shutil.copy2(source_path_server, dest_path_server)
        except Exception as e:
            print(f"cp: error copying '{source}': {e}", file=sys.stderr)

if __name__ == '__main__':
    main()
