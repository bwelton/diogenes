# bsub -nnodes 1 -G guests -W 100 /g/g17/welton2/repo/cuda_deduplicator/settings_files/cumf_als/launch_ts.sh
cd /g/g17/welton2/scratch/nfs/apps/qbox_dir/qbox-public/src/gold-bench
source /g/g17/welton2/scratch/nfs/apps/setupEnv.sh
source /g/g17/welton2/scratch/nfs/apps/qbox_dir/qbox-public/src/setupSpecialEnv.sh
cp /g/g17/welton2/repo/cuda_deduplicator/settings_files/qbox/TS_settings.ini /g/g17/welton2/scratch/nfs/apps/qbox_dir/qbox-public/src/gold-bench/settings.ini
module load cuda/8.0
time source /g/g17/welton2/scratch/nfs/apps/qbox_dir/qbox-public/src/gold-bench/run_wait.sh
