/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License.
 */

#include "meta/processors/job/RebuildEdgeJobExecutor.h"

namespace nebula {
namespace meta {

folly::Future<Status> RebuildEdgeJobExecutor::executeInternal(HostAddr&& address,
                                                              std::vector<PartitionID>&& parts) {
  folly::Promise<Status> pro;
  auto f = pro.getFuture();
  adminClient_
      ->addTask(cpp2::AdminCmd::REBUILD_EDGE_INDEX,
                jobId_,
                taskId_++,
                space_,
                std::move(address),
                taskParameters_,
                std::move(parts),
                concurrency_)
      .then([pro = std::move(pro)](auto&& t) mutable {
        CHECK(!t.hasException());
        auto status = std::move(t).value();
        if (status.ok()) {
          pro.setValue(Status::OK());
        } else {
          pro.setValue(status.status());
        }
      });
  return f;
}

}  // namespace meta
}  // namespace nebula
