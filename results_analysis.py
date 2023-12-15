import os
import numpy as np
import matplotlib.pyplot as plt

class BenchmarkAnalyzer:
    def __init__(self, *folder_paths):
        self.folder_paths = folder_paths
        self.data = {os.path.basename(folder): {} for folder in folder_paths}

    def process_folders(self):
        for folder_path in self.folder_paths:
            folder_name = os.path.basename(folder_path)
            for file_name in os.listdir(folder_path):
                if file_name.startswith("mobilenet") and file_name.endswith(".txt"):
                    self.process_file(folder_name, folder_path, file_name)

    def process_file(self, folder_name, folder_path, file_name):
        # Extract the number of threads from the file name
        threads = int(file_name.split("_")[-1].replace("threads.txt", ""))
        
        # Read and process the file
        with open(os.path.join(folder_path, file_name), 'r') as file:
            lines = file.readlines()[3:]  # Skip the first three lines
            times = [float(line.strip()) for line in lines]
            mean_time = np.mean(times)
            
            # Save in the data dictionary
            self.data[folder_name][threads] = mean_time

    def get_data(self):
        return self.data
    
    def plot_benchmark_times(self):
        # Prepare data for plotting
        threads = sorted(next(iter(self.data.values())).keys())
        fig, ax = plt.subplots()
        for i, (schedule, times) in enumerate(self.data.items()):
            ax.bar(np.array(threads) + i*0.2, [times[thread] for thread in threads], width=0.2, label=schedule)

        # Customizing the plot
        ax.set_xlabel('Number of Threads')
        ax.set_ylabel('Mean Benchmark Time (ms)')
        ax.set_title('Benchmark Times by Scheduling Type')
        ax.set_xticks(np.array(threads) + 0.2)
        ax.set_xticklabels(threads)
        ax.legend()
        ax.grid(True)
        plt.show()

    def plot_speedup_graph(self):
        # Prepare data for plotting
        fig, ax = plt.subplots()
        max_threads = max(max(self.data[schedule].keys()) for schedule in self.data)

        for schedule, times in self.data.items():
            one_thread_time = times[1]
            speedups = [one_thread_time / times[thread] for thread in sorted(times.keys())]
            ax.plot(sorted(times.keys()), speedups, label=schedule, marker='o')

        # Adding the ideal linear speedup line (y = x)
        ideal_speedup = range(1, max_threads + 1)
        ax.plot(ideal_speedup, ideal_speedup, label='Ideal Speedup (y=x)', linestyle='--', color='grey')

        # Customizing the plot
        ax.set_xlabel('Number of Threads')
        ax.set_ylabel('Speedup')
        ax.set_title('Speedup by Scheduling Type')
        ax.legend()
        ax.grid(True)
        plt.show()

def main():
    # Specify the folder paths here
    folder_paths = ["BenchmarkTimes/Dynamic_Scheduling", "BenchmarkTimes/Static_Scheduling", "BenchmarkTimes/Guided_Scheduling"]
    analyzer = BenchmarkAnalyzer(*folder_paths)
    analyzer.process_folders()
    analyzer.plot_benchmark_times()
    analyzer.plot_speedup_graph()

if __name__ == "__main__":
    main()



