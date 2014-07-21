import os
import sys

activate_this = '/home/matthew/Documents/classifier/website/venv/bin/activate_this.py'
execfile(activate_this, dict(__file__=activate_this))

sys.path.insert(0, '/home/matthew/Documents/classifier/website')

from hello import app as application