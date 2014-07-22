import os
from flask import Flask, render_template, request, redirect, url_for, send_from_directory
from werkzeug.utils import secure_filename

UPLOAD_FOLDER = '/home/matthew/Documents/classifier/website/uploads'
ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg', 'gif', 'tif', 'tiff'])

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
#app.debug = True
#@app.route('/')
#def index():
#    return 'Index Page'

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

@app.route('/')
def root():
#    return 'working'
    return render_template('index.html')

@app.route('/hello')
def hello():
    return 'Hello World! This is a legacy page for testing.'

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/me')
def me():
    return render_template('me.html')

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

@app.route('/butterflies',methods=['GET','POST'])
def butterflies():
    if request.method == 'POST':
        file = request.files['file']
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(url_for('uploaded_file',filename=filename))
    return render_template('butterflies.html')

@app.route('/wasps')
def wasps():
    return render_template('wasps.html')

if __name__ == '__main__':
    app.run()
