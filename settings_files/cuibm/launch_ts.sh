# bsub -nnodes 1 -G guests -W 100 /g/g17/welton2/repo/cuda_deduplicator/settings_files/cumf_als/launch_ST.sh
cd /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000
source /g/g17/welton2/scratch/nfs/apps/setupEnv.sh
cp /g/g17/welton2/repo/cuda_deduplicator/settings_files/cuibm/TS_settings.ini /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000/settings.ini
module load cuda/9.2.148
time source /g/g17/welton2/scratch/nfs/apps/cuIBM/examples/lidDrivenCavity/Re5000/run_wait.sh