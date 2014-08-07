import os, sys, classifier, wikipedia
from flask import Flask, render_template, request, redirect, url_for, send_from_directory
from werkzeug.utils import secure_filename

UPLOAD_FOLDER = '/home/matthew/Documents/classifier/website/uploads'
ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg', 'gif', 'tif', 'tiff'])
SVM_PATH = '/home/matthew/Documents/classifier/Classifier/SVMS_1_2014-08-04.16:30:24'
VOCAB_PATH = '/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-08-05.19:46:07.yml'

 

#classifier.classify("/home/matthew/Documents/classifier/Classifier/SVMS_12014-07-24.14:30:30","/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-16.18:03:34.yml","/home/matthew/Documents/Data/UK-Leps.images/Aglais-urticae/Aglais-urticae-01.tif")

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
            filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(filepath)
            filename = 'http://146.169.45.105/uploads/' + filename
            res = classifier.classify(SVM_PATH,VOCAB_PATH, filepath)
            page = wikipedia.page(res)
            url = page.url
            wiki = page.content
            try:
                index = wiki.index('\n\n\n== See')
                wiki = wiki[:index]
                wiki = wiki.replace('\n\n\n==','<h2>')
                wiki = wiki.replace('==\n','</h2>')
            except ValueError:
                wiki = wiki.replace('\n\n\n==','<h2>')
                wiki = wiki.replace('==\n','</h2>')
            #return redirect(url_for('uploaded_file',filename=filename))
            return render_template('results.html',res=res,filename=filename,wiki=wiki,url=url)
    return render_template('butterflies.html')

@app.route('/wasps')
def wasps():
    return render_template('wasps.html')

if __name__ == '__main__':
    app.run()
