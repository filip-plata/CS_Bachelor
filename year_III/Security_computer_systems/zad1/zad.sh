#!/usr/bin/env bash

groupadd staff
useradd master

for f in {1..3}
do
	classID="class-e${f}"
	groupadd ${classID}
	for id in {01..15}
	do
		userID="424$(($f * 2))${id}"
		userPublic="/home/${userID}/public"
		userHome="/home/${userID}"

		useradd -G ${classID} -m  ${userID}

		mkdir ${userPublic}
		chown -R ${userID} ${userHome}

		setfacl -d -m g:${classID}:r-x ${userPublic}
		setfacl -m g:${classID}:r-x ${userPublic}
		setfacl -m g:${classID}:--x ${userHome}

		setfacl -d -m g:staff:rwx ${userPublic}
		setfacl -m g:staff:r-x ${userPublic}
		setfacl -m g:staff:--x ${userHome}

		setfacl -d -m u:master:rwx ${userPublic}
		setfacl -m u:master:rwx ${userPublic}
		setfacl -m u:master:--x ${userHome}

		setfacl -d -m other:--- ${userHome}
		setfacl -m other:--- ${userHome}
	done
done

