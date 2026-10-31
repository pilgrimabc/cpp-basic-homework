import subprocess
import sys

count = int(sys.argv[1]) if len(sys.argv) > 1 else 1

progs = {
    "One-thread": "./build/test",
    "Mutex": "./build/mt1",
    "Aync local dics": "./build/mt2"
}

file = "gatsby.txt"
file_count_times = count

def run_benchmark():

    file_args = [file] * file_count_times

    print(f"For {file_count_times} files:")
    print("="*60)

    for name, path in progs.items():
        result = subprocess.run([path] + file_args, stdout=subprocess.PIPE, text=True)

        output_lines = result.stdout.splitlines()
        for line in output_lines:
            if "Elapsed time" in line:
                print(f"Result for {name} is {line}")

    print("="*60)


if __name__ == "__main__":
    run_benchmark()