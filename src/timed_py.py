import sys
import time

argc = len(sys.argv)
argv = sys.argv

delay = 0.05

if (argc > 1):
    delay = int(argv[1])/1000.0

query = ""
while (query != "quit"):
    query = raw_input()
    print >> sys.stderr, ">>> received:", query
    s_t = time.time()
    while (time.time() - s_t < delay):
        pass
    print "echo: {}".format(query)
    sys.stdout.flush()
