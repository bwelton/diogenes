# bsub -nnodes 1 -G guests -W 100 /g/g17/welton2/repo/cuda_deduplicator/settings_files/cumf_als/launch_mr.sh
cd /g/g17/welton2/scratch/nfs/apps/cumf_als
source /g/g17/welton2/scratch/nfs/apps/setupEnv.sh
cp /g/g17/welton2/repo/cuda_deduplicator/settings_files/cumf_als/MR_settings.ini /g/g17/welton2/scratch/nfs/apps/cumf_als/settings.ini
module load cuda/9.2.148
time source /g/g17/welton2/scratch/nfs/apps/cumf_als/run_wait.sh