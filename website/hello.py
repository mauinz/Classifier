from flask import Flask
from flask import render_template

app = Flask(__name__)

#@app.route('/')
#def index():
#    return 'Index Page'

@app.route('/')
def root():
#    return 'working'
    return render_template('index.html')

@app.route('/hello')
def hello():
    return 'Hello World'

if __name__ == '__main__':
    app.run()
