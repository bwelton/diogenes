#ifndef MEMGRAPH_ALLOCATIONTRACKER
#define MEMGRAPH_ALLOCATIONTRACKER 1
#include <inttypes.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace MemGraphBuild{
    class AllocationSite;
    class FreeSite;
    class TransferSite;
    class AllocationTracker;
    typedef std::shared_ptr<FreeSite> FreeSitePtr;
    typedef std::shared_ptr<AllocationSite> AllocSitePtr;
    typedef std::shared_ptr<TransferSite> TransSitePtr;
    typedef std::shared_ptr<AllocationTracker> AllocTrackPtr;
    class AllocationSite {
        public:
            uint64_t stackID;
            uint64_t callCount;
            uint64_t freed;
            std::unordered_set<FreeSitePtr> freeSites;
            AllocationSite(uint64_t s) {
                stackID = s;
                callCount = 0;
                freed = 0;
            };
    };

    class FreeSite {
        public:
            uint64_t stackID;
            uint64_t callCount;
            uint64_t unknownAlloc;
            std::unordered_set<AllocSitePtr> allocSites;
            FreeSite(uint64_t s) {
                stackID = s;
                callCount = 0;
                unknownAlloc = 0;
            };
    };

    class TransferSite {
        public:
            uint64_t stackID;
            uint64_t callCount;
            uint64_t unknownAlloc;
            std::unordered_set<AllocSitePtr> allocSites;
            TransferSite(uint64_t s) {
                stackID = s;
                callCount = 0;
                unknownAlloc = 0;               
            };
    };
    class AllocationTracker {
        public:
            void HandleAllocation(uint64_t stackAddr, uint64_t memAddr, uint64_t size);
            void HandleFree(uint64_t stackAddr, uint64_t memAddr);
            void HandleTransfer(uint64_t stackAddr, uint64_t cpuAddress, uint64_t size);
            char * SerializeData(uint64_t & size);

        private:
            std::map<uint64_t, std::pair<uint64_t, AllocSitePtr>> _addrToAlloc;
            std::unordered_map<uint64_t, AllocSitePtr> _stackToAlloc;
            std::unordered_map<uint64_t, FreeSitePtr> _stackToFree;
            std::unordered_map<uint64_t, TransSitePtr> _stackToTrans;

    };
    AllocTrackPtr GetGlobalTracker();

};
#endif