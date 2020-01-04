FROM ubuntu

RUN export uid=1000 gid=1000 && \
    mkdir -p /home/developer && \
    echo "developer:x:${uid}:${gid}:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd && \
    echo "developer:x:${uid}:" >> /etc/group && \
    chown ${uid}:${gid} -R /home/developer

RUN apt-get update
RUN apt-get install libgtk-3-0 -y

ADD gui3/gui3 /home/developer/game/
ADD build/siktacka-client /home/developer/game/
ADD start.sh /home/developer/game/start

RUN chmod +x /home/developer/game/start
RUN chmod +x /home/developer/game/
RUN chmod +x /home/developer/game/
RUN chown developer:developer /home/developer

USER developer
ENV HOME /home/developer
WORKDIR /home/developer/game/

CMD ["/home/developer/game/start"]
