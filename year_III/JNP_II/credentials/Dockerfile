FROM python:3.6

# Copy in your requirements file
ADD Pipfile /Pipfile
ADD Pipfile.lock /Pipfile.lock

# OR, if you’re using a directory for your requirements, copy everything (comment out the above and uncomment this if so):
# ADD requirements /requirements

# Install build deps, then run `pip install`, then remove unneeded build deps all in a single step. Correct the path to your production requirements file, if needed.
RUN apt-get update && \
    apt-get install -y && \
    pip3 install uwsgi
RUN pip install pipenv

# Copy your application code to the container (make sure you create a .dockerignore file if any large files or directories should be excluded)
RUN mkdir /credentials/
WORKDIR /credentials/
ADD . /credentials/
RUN pipenv install --system

# uWSGI will listen on this port
EXPOSE 8040

# Start uWSGI
CMD ["uwsgi", "--ini", "/credentials/uwsgi.ini"]
