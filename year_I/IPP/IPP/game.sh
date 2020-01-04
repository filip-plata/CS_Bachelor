#!/bin/bash

ROOT_PROCESS=$$

# Trapy na kody wysyłane przez procesy
# odpalające GUI i AI.
trap "cleanup; exit 1"  SIGUSR1
trap "cleanup; exit 0" SIGUSR2

SIZE=10
TURNS=100
DELAY=1
DOES_AI_PLAY[1]="NO"
DOES_AI_PLAY[2]="NO"
AI_PATH[1]=""
AI_PATH[2]=""
KING_COLUMN[1]=-1
KING_ROW[1]=-1
KING_ROW[2]=-1
KING_COLUMN[2]=-1
CURRENT_PLAYER=1
OPPOSITE_PLAYER=2
AI_1_PID=0
AI_2_PID=0
GUI_PID=0

MAKE_LOG="NO"
positionError="Wrong format of a king's position"

# Numery deskryptorów.
PLAYER_INPUT[1]=3
PLAYER_OUTPUT[1]=4
PLAYER_INPUT[2]=5
PLAYER_OUTPUT[2]=6
GUI_INPUT=7
CLEANUP_FLAG=0

function cleanup {
    CLEANUP_FLAG=1
    EXIT=0;
    echo "killing continues"
    if [ ! ${AI_1_PID} -eq 0 ]
    then
        if kill -0 ${AI_1_PID}; then EXIT=1; fi
        #pkill -9 -P ${AI_1_PID}
        #kill -9 "$AI_1_PID"
    fi
    if [ ! ${AI_2_PID} -eq 0 ]
    then
        if kill -0 ${AI_2_PID}; then EXIT=1; fi
        #pkill -9 -P ${ROOT_PROCESS}
        #kill -9 "$AI_2_PID"
    fi
    for i in `ps -ef| awk '$3 == '${ROOT_PROCESS}' { print $2 }'`
    do
        for j in `ps -ef| awk '$3 == '${i}' { print $2 }'`
        do
            kill -9 $j
        done
        kill -9 $i
    done
    #pkill -9 -P ${ROOT_PROCESS}
    rm -f flag
    if [[ $EXIT == "1" ]]; then exit 1; fi
}

function errcho() {
    echo "$@" 1>&2
}

# Zwraca 1 jeśli pierwszy argument jest liczbą
# 0 przeciwnie
function isNumber {
    case ${1#[-+]} in
        ''|*[!0-9]* ) return 1 ;;
        * ) if [ $1 -gt 2500000000 ]
        then
            return 1
        else
            return 0
        fi ;;
    esac
}

# Sprawdza, czy pierwszy argument jest postaci
# [liczba1],[liczba2]
function rawPositionValidation {
    res="${1//[^,]}"

    if [[ ! "${#res}" == 1 ]]; then return 0; fi

    firstNumber=${1%,*}
    secondNumber=${1#*,}

    if ! isNumber ${firstNumber}; then return 0; fi
    if ! isNumber ${secondNumber}; then return 0; fi

    return 1
}


function isInsideBoard {
    if [ "$1" -gt "$SIZE" ]; then return 1; fi
    if [ "$1" -lt 1 ]; then return 1; fi
    return 0;
}


# Wczytuje argumenty do zmiennych.
function readArguments {
    while [[ $# > 0 ]]
    do
	key="$1"
	case $key in
            -n)
        if ! isNumber "$2"; then errcho "Invalid argument"; exit 1; fi
		SIZE="$2"
		shift
		;;
            -k)
		TURNS="$2"
		shift
		;;
            -s)
		DELAY="$2"
		shift
		;;
            -ai1)
		AI_PATH[1]="$2"
		DOES_AI_PLAY[1]="YES"
		shift
		;;
            -ai2)
		AI_PATH[2]="$2"
		DOES_AI_PLAY[2]="YES"
		shift
		;;
            -p1)
        if rawPositionValidation "$2"; then errcho ${positionError}; exit 1; fi
		KING_COLUMN[1]=${2%,*}
		KING_ROW[1]=${2#*,}
		shift
		;;
            -p2)
        if rawPositionValidation "$2"; then errcho ${positionError}; exit 1; fi
		KING_COLUMN[2]=${2%,*}
		KING_ROW[2]=${2#*,}
		shift
		;;
		    *)
		    errcho "Someone tried to cheat"
		    exit 1
	esac
	shift
    done
}

function distanceCooridinate {
    distance=$(($1 - $2))
    distance=$(( distance > 0 ? distance : -distance))

    echo ${distance}
}
function distanceMaximumMetric {
    distanceX=$(distanceCooridinate $1 $3)
    distanceY=$(distanceCooridinate $2 $4)

    echo $((distanceX > distanceY ? distanceX : distanceY ))
}

# Wystarczy sprawdzić odległość do czterech skrajnych
# punktów z możliwych pozycji króla
function canPutAnotherKing {

    if [ $(distanceMaximumMetric $1 $2 1 1) -lt 8 ] &&
    [ $(distanceMaximumMetric $1 $2 $((SIZE-3)) 1) -lt 8 ] &&
    [ $(distanceMaximumMetric $1 $2 1 SIZE) -lt 8 ] &&
    [ $(distanceMaximumMetric $1 $2 $((SIZE-3)) SIZE) -lt 8 ]
    then
        return 1
    fi
    return 0
}

# Wstawia jednego króla w
# losowe miejsce spełniające zasady gry.
function putAnotherKing {
    PLAYER=$1

    while [ true ]
    do
        colLIM=$((SIZE - 3))
        KING_COLUMN[$PLAYER]=$(( (RANDOM % colLIM) + 1))
        KING_ROW[$PLAYER]=$(( (RANDOM % SIZE) + 1))
        kr=${KING_ROW[PLAYER]}
        kc=${KING_COLUMN[PLAYER]}
        if [ $(distanceMaximumMetric $2 $3 $kr $kc) -gt 8 ]
        then
            break
        fi
    done
}

# Sprawdza poprawność danych wejściowych.
# W szczególności :
# - czy obie mieszczą się w planszy
# - jeśli podano obie pozycje, czy są w stosownej odległości
# - jeśli podano jedną, czy można dolosować drugą zgodnie z zasadami
function validateInputData {

    if [ ${KING_ROW[1]} -eq "-1" ] && [ ${KING_ROW[2]} -eq "-1" ]
    then
        getRandomPosition
        return 0
    fi

    # Użytkownik podał oba parametry p1 i p2
    if [ ! ${KING_ROW[1]} -eq "-1" ] && [ ! ${KING_ROW[2]} -eq "-1" ]
    then
        if  ! isInsideBoard ${KING_ROW[1]}  ||
        ! isInsideBoard ${KING_COLUMN[1]} ||
        ! isInsideBoard ${KING_COLUMN[2]} ||
        ! isInsideBoard ${KING_ROW[2]}
        then
            errcho "One of row or column numbers is not contained by the board"
            exit 1
        fi
        maximum=$(distanceMaximumMetric ${KING_ROW[1]} ${KING_COLUMN[1]}\
        ${KING_ROW[2]} ${KING_COLUMN[2]})

        if [ "$maximum" -lt 8 ]; then errcho "King are too close"; exit 1; fi
        return 0
    fi

    #Użytkownik podał jeden parametr. Ustalamy, dla którego gracza
    PLAYER=1
    OPPONENT=2

    if [ ${KING_ROW[2]} -eq "-1" ]
    then
        PLAYER=2
        OPPONENT=$((3 - PLAYER))
    fi

    if  ! isInsideBoard ${KING_ROW[OPPONENT]}  ||
     ! isInsideBoard ${KING_COLUMN[OPPONENT]}
    then
        errcho "One of row or column numbers is not contained by the board"
        exit 1
    fi


    error="Not able to insert another king to the board"
    if ! canPutAnotherKing ${KING_ROW[OPPONENT]} ${KING_COLUMN[OPPONENT]}
    then
        errcho "$error"
        exit 1
    fi
    putAnotherKing ${PLAYER} ${KING_ROW[OPPONENT]} ${KING_COLUMN[OPPONENT]}
}

# Funkcja losująca początkowe położenie.
# Królowie lądują w pierwszym i ostatnim rzędzie,
# w losowej kolumnie.
function getRandomPosition {
    if [ ${KING_COLUMN[1]} -eq -1 ]
    then
	KING_ROW[1]=1
	KING_COLUMN[1]=$(shuf -i 1-$((SIZE-3)) -n 1)
    fi
    
    if [ ${KING_COLUMN[2]} -eq -1 ]
    then
	KING_ROW[2]=${SIZE}
	KING_COLUMN[2]=$(shuf -i 1-$((SIZE-3)) -n 1)
    fi
}

# KOnstruuje inity.
function prepareInits {
    for i in 1 2
    do
	INIT[i]="INIT $SIZE $TURNS $i ${KING_COLUMN[1]} ${KING_ROW[1]}\
 ${KING_COLUMN[2]} ${KING_ROW[2]}"
    done
}


function pipeAndFileDescriptor {
    PIPE=$(mktemp -u)
    mkfifo ${PIPE}
    eval "exec $1<>${PIPE}"
    rm ${PIPE}
}

function monitorGUI {
    ./sredniowiecze_gui_with_libs.sh $3 $4 <&$1 >&$2
    CODE=$?
    sleep 0.1
    read CLEANUP_FLAG < flag
    if [ $CLEANUP_FLAG -eq 1 ]; then exit; fi
    if [ $CODE -eq 0 ]
    then
        kill -SIGUSR2 $ROOT_PROCESS; exit
    else
        kill -SIGUSR1 $ROOT_PROCESS; exit
    fi
}

function monitorAI {
    if [[ ! -x "$1" ]]
    then
        kill -SIGUSR1 $ROOT_PROCESS; exit
    fi

    $1 <&$2 >&$3
    CODE=$?

    read CLEANUP_FLAG < flag
    if [ $CLEANUP_FLAG -eq 1 ]; then exit; fi

    if [ $CODE -gt 3 ]
    then
        echo "1" > flag
        kill -SIGUSR1 $ROOT_PROCESS; exit
    fi
}

function createPipes {
    for f in 3 4 5 6 7
    do
	    pipeAndFileDescriptor ${f}
    done
}

# Uruchamia procesy AI i GUI z odpowiednimi
# przekierowaniami wejścia i wyjścia.
function attachPipesToProcesses {

    echo ${INIT[1]} >&"${PLAYER_INPUT[1]}"
    echo ${INIT[2]} >&"${PLAYER_INPUT[2]}"


    if [ ${DOES_AI_PLAY[1]} == "YES" ] && [ ${DOES_AI_PLAY[2]} == "YES" ]
    then
	STANDARD_OUTPUT=1
	echo ${INIT[1]} >&"$GUI_INPUT"
	echo ${INIT[2]} >&"$GUI_INPUT"
	monitorGUI ${GUI_INPUT} ${STANDARD_OUTPUT} &
	GUI_PID="$!"
	monitorAI ${AI_PATH[1]} ${PLAYER_INPUT[1]} ${PLAYER_OUTPUT[1]} &
	AI_1_PID="$!"
	monitorAI ${AI_PATH[2]} ${PLAYER_INPUT[2]} ${PLAYER_OUTPUT[2]} &
	AI_2_PID="$!"
    else
	DELAY=0
	if [ ${DOES_AI_PLAY[1]} == "YES" ]
	then
	    ARGS="-human2"
	    GUI=2
	    AI=1
	else
	    ARGS="-human1"
	    GUI=1
	    AI=2
	fi
	echo ${INIT[AI]} >&"${PLAYER_INPUT[GUI]}"
	monitorGUI "${PLAYER_INPUT[GUI]}" "${PLAYER_OUTPUT[GUI]}" $ARGS &
	sleep 1
	monitorAI "${AI_PATH[AI]}" "${PLAYER_INPUT[AI]}" "${PLAYER_OUTPUT[AI]}" &
	AI_1_PID="$!"
    fi
}

function switchPlayers {
    CURRENT_PLAYER=$((3-CURRENT_PLAYER))
    OPPOSITE_PLAYER=$((3-OPPOSITE_PLAYER))
}

# Główna funkcja gry. Dwa przypadki są zasadzniczo różne-
# gdy gra jakieś AI i gdy żadne nie gra. W tym drugim
# logika jest uwspólniona, przez co np. w rozgrywce 
# AI vs GUI wypisywane są niepotrzebnie dane do pewnego
# deskryptora.

playGame () {
    if [ "${DOES_AI_PLAY[1]}" == "NO" ] && [ "${DOES_AI_PLAY[2]}" == "NO" ]
    then
	pipeAndFileDescriptor ${GUI_INPUT}
	echo ${INIT[1]} >&"$GUI_INPUT"
	echo ${INIT[2]} >&"$GUI_INPUT"
	monitorGUI $GUI_INPUT 1 -human1 -human2
    else
	attachPipesToProcesses
	while [ true ]
	do
        unset line
        while  [ true ]
        do
            read line <&"${PLAYER_OUTPUT[CURRENT_PLAYER]}"
		    echo $line >&"${GUI_INPUT}"
		    echo $line >&"${PLAYER_INPUT[OPPOSITE_PLAYER]}"
		    if [[ "${MAKE_LOG}" -eq "YES" ]]
		    then
		        echo $line >> log
		    fi
		    if [ "${line}" == "END_TURN" ]
		    then
                break;
		    fi
        done
        switchPlayers
        sleep ${DELAY}
    done
     fi
 }

# Jeśli MAKE_LOG jest ustawione na YES, to
# do pliku log wypisywany jest przebieg całej
# rozgrywki.
function makeLog {
    MAKE_LOG="YES"
    echo "NEXT_GAME" >> log
}

# Dla zwiększenia czytelności dodałem main
function main {
    readArguments "$@"
    validateInputData
    prepareInits
    createPipes
    makeLog
    playGame
}

# Jedyna funkcja uruchamiana z poziomu skryptu.
main "$@"
