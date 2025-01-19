import os
import sys

def generate_error_codes(include_dir, output_file):
    if not os.path.isdir(include_dir):
        print(f"Error: Directory '{include_dir}' does not exist.")
        sys.exit(1)

    files = [f for f in os.listdir(include_dir) if f.endswith('.h')]
    if not files:
        print(f"Error: No header files found in directory '{include_dir}'.")
        sys.exit(1)

    files.sort()

    try:
        with open(output_file, 'w') as f:
            f.write('#ifndef __ERROR_CODE_AUTOMAKE_H\n')
            f.write('#define __ERROR_CODE_AUTOMAKE_H\n\n')
            f.write('enum error_element {\n')

            # Special case for cc_error_code.h
            if 'cc_error_code.h' in files:
                f.write('    CC_ERROR_CODE_H = 0xA0,\n')
                files.remove('cc_error_code.h')

            # Assign remaining files sequential values starting from 0xA1
            for i, file in enumerate(files):
                enum_name = file.replace('.', '_').upper()
                f.write(f'    {enum_name} = 0xA{i + 1:01X},\n')

            f.write('};\n\n')
            f.write('#endif // __ERROR_CODE_AUTOMAKE_H\n')
    except Exception as e:
        print(f"Error: Failed to write to '{output_file}'. {e}")
        sys.exit(1)

if __name__ == "__main__":
    include_dir = os.path.join(os.path.dirname(__file__), '.')
    output_file = os.path.join(include_dir, 'error_code_automake.h')
    generate_error_codes(include_dir, output_file)