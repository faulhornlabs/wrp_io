#pragma once
#include <asio/io_context.hpp>
//#include <asio/posix/win_io_context.hpp>

namespace asio {
    
namespace posix {

/// Provides stream-oriented descriptor functionality.
/**
 * The posix::basic_stream_descriptor class template provides asynchronous and
 * blocking stream-oriented descriptor functionality.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 *
 * @par Concepts:
 * AsyncReadStream, AsyncWriteStream, Stream, SyncReadStream, SyncWriteStream.
 */


template <typename Executor = int>
class basic_stream_descriptor
{
public:
    basic_stream_descriptor(const asio::io_context& ctx) {}
};

}
}