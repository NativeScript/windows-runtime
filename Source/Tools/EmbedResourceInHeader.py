#!/usr/bin/python

import argparse

COLUMNS_PER_ROW = 12


def main():
    parser = argparse.ArgumentParser(description='Creates a .h file that defines an array of bytes.')
    parser.add_argument('--resource', required=True, help='The path to the resource file')
    parser.add_argument('--header', required=True, help='The path to the header file to create')
    parser.add_argument('--name', required=True, help='The name of the array to create in the file')
    args = parser.parse_args()

    with open(args.resource, mode='rb') as resource_file:
        resource_bytes = bytearray(resource_file.read())

        with open(args.header, mode='w') as header_file:
            name_splitted = args.name.split('.')

            for i in range(0, len(name_splitted) - 1):
                header_file.write('namespace %s {\n' % name_splitted[i])

            header_file.write('const char %s[] = {\n' % name_splitted[-1])

            current_column = 0
            for i in range(len(resource_bytes)):
                if current_column == 0:
                    header_file.write('    ')

                header_file.write('%#04x' % resource_bytes[i])

                if i != len(resource_bytes) - 1:
                    header_file.write(',')

                    if current_column < COLUMNS_PER_ROW - 1:
                        header_file.write(' ')

                current_column += 1

                if current_column == COLUMNS_PER_ROW or i == len(resource_bytes) - 1:
                    header_file.write('\n')
                    current_column = 0

            header_file.write('};\n')

            for i in range(0, len(name_splitted) - 1):
                header_file.write('}\n')


if __name__ == '__main__':
    main()
