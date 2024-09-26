declare -a sched=("static" "dynamic" "guided")
declare -a chunk=("1" "4")
for s in "${sched[@]}"; do 
    for c in "${chunk[@]}"; do 
        echo Sched=$s Chunk=$c
        cat sched.cpp | sed -e "s/SCHEDULER/$s/g" | sed -e "s/CHUNK_SIZE/$c/g" > sched_comp.cpp
        bash comrun.sh sched_comp.cpp
    done
done

