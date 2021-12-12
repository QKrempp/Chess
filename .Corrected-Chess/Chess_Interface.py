from subprocess import Popen, PIPE
#Or just:
coups = open("Game.txt", 'r').read().split(sep='\n')[:-1]
args = ["./Chess.xpp"] + coups
process = Popen(args, stdout=PIPE)
process.wait()
for output in process.stdout.read().strip().decode().splitlines():
    print(output)
#(stdout, stderr) = process.communicate()
