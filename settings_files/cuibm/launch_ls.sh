# bsub -nnodes 1 -G guests -W 210 /g/g17/welton2/repo/cuda_deduplicator/settings_files/cuibm/launch_mr.sh
cd /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000
source /g/g17/welton2/scratch/nfs/apps/setupEnv.sh
cp /g/g17/welton2/repo/cuda_deduplicator/settings_files/cuibm/LS_settings.ini /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000/settings.ini
module load cuda/9.2.148
time source /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000/run_wait.sh