if [ $1 ]
then
  echo Without load: > $1

  for k in `seq 3`
  do
    (time grep size_t /usr/src/minix/servers/*/*c > /dev/null) 2>> $1
  done

  grep -Rs size_t /usr/ > /dev/null &
  PID1=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID2=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID3=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID4=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID5=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID6=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID7=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID8=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID9=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID10=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID11=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID12=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID13=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID14=$!

  grep -Rs size_t /usr/ > /dev/null &
  PID15=$!

  echo Wait 20s for greps to use up their startup tokens
  sleep 20
  echo Go

  echo With load: >> $1

  for k in `seq 3`
  do
   (time grep size_t /usr/src/minix/servers/*/*c > /dev/null) 2>> $1
  done

  cat $1

  kill $PID1
  kill $PID2
  kill $PID3
  kill $PID4
  kill $PID5
  kill $PID6
  kill $PID7
  kill $PID8
  kill $PID9
  kill $PID10
  kill $PID11
  kill $PID12
  kill $PID13
  kill $PID14
  kill $PID15
else
  echo USAGE: $0 logfilename
fi
