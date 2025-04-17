# -*- coding: utf-8 -*-
import re
import os
import json

def parse_generate_body_headers():
    class_list = []
    
    script_dir = os.path.dirname(os.path.abspath(__file__))
    header_dir = os.path.abspath(os.path.join(script_dir, "..", "250304_WinAPI"))

    for file in os.listdir(header_dir):
        full_path = os.path.join(header_dir, file)

        if not os.path.isfile(full_path):
            continue
        if file.lower() == "config.h": 
            continue
        ext = os.path.splitext(file)[1].lower()
        if ext not in [".h", ".hpp"]:
            continue

        # lists
        encodings = ['utf-8', 'cp949', 'euc-kr']
        content = None

        for enc in encodings:
            try:
                with open(full_path, encoding=enc) as f:
                    content = f.readlines()
                break  
            except UnicodeDecodeError:
                continue  

        if content is None:
            print(f"faild encoding : {full_path}")
            continue

        # extract
        for i, line in enumerate(content):
            # if "GENERATE_BODY()" in line:
            #     print(f"find generate body {full_path}")
            #     for j in range(i, -1, -1):
            #         match = re.match(r'\s*class\s+(\w+)', content[j])
            #         if match:
            #             class_name = match.group(1)
            #             class_list.append(class_name)
            #             print(f"find class : {class_name} ({file})")
            #             break

            if "GENERATE_BODY(" in line:
                # 클래스 이름 추출
                class_name = None
                for j in range(i, -1, -1):
                    match = re.match(r'\s*class\s+(\w+)', content[j])
                    if match:
                        class_name = match.group(1)
                        break

                # 매크로 인자 추출
                body_match = re.search(r'GENERATE_BODY\((.*?)\)', line)
                if not class_name or not body_match:
                    continue

                args = [arg.strip().strip('"') for arg in body_match.group(1).split(',')]
                if len(args) != 3:
                    print(f"error : {file} / {class_name}")
                    continue

                atlas_path = args[0]
                cell_width = int(args[1])
                cell_height = int(args[2])

                meta = {
                    "name": class_name,
                    "atlasPath": atlas_path,
                    "cellSize": {
                        "x": cell_width,
                        "y": cell_height
                    }
                }

                class_list.append(meta)

    return class_list

classes = parse_generate_body_headers()
print("Detected classes:", classes)

def save_to_json(data, filename="object_meta.json"):

    script_dir = os.path.dirname(os.path.abspath(__file__))  # 현재 .py 파일 위치
    output_path = os.path.join(script_dir, filename)

    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)
    print(f"save complete : {output_path}")

def save_register_header(data, filename="ObjectRegister.h"):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    header_dir = os.path.abspath(os.path.join(script_dir, "..", "250304_WinAPI"))
    output_path = os.path.join(header_dir, filename)

    with open(output_path, "w", encoding="euc-kr") as f:
        f.write("#pragma once\n")
        f.write("// Auto Generate : DO NOT MODIFY\n\n")
        
        # ObjectFactory 포함 먼저
        f.write('#include "ObjectFactory.h"\n')

        # 클래스별 헤더 인클루드
        for entry in data:
            f.write(f'#include "{entry["name"]}.h"\n')

        f.write("\n")

         # REGISTER_OBJECT() 호출
        for entry in data:
            f.write(f"REGISTER_OBJECT({entry['name']})\n")
    print(f"Saved object registration header to: {output_path}")

if __name__ == "__main__":
    metas = parse_generate_body_headers()
    save_to_json(metas)
    save_register_header(metas)
