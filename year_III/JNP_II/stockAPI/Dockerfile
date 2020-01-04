FROM python:3.6

# Copy in your requirements file
ADD Pipfile /Pipfile
ADD Pipfile.lock /Pipfile.lock

# OR, if you’re using a directory for your requirements, copy everything (comment out the above and uncomment this if so):
# ADD requirements /requirements

# Install build deps, then run `pip install`, then remove unneeded build deps all in a single step. Correct the path to your production requirements file, if needed.
RUN apt-get update && \
    apt-get install -y cron && \
    pip3 install uwsgi pipenv

# Copy crontab
ADD crontab /etc/cron.d/stock-tick
RUN chmod 0644 /etc/cron.d/stock-tick
RUN touch /var/log/cron.log

# Copy your application code to the container (make sure you create a .dockerignore file if any large files or directories should be excluded)
RUN mkdir /stockAPI/
WORKDIR /stockAPI/
ADD . /stockAPI/
RUN pip3 install -r requirements.txt
RUN chmod +x /stockAPI/start.sh

# uWSGI will listen on this port
EXPOSE 8060

# Start uWSGI
CMD ["./start.sh"]
