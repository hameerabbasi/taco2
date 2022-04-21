#ifndef XSPARSE_LEVELS_OFFSET_HPP
#define XSPARSE_LEVELS_OFFSET_HPP

#include <tuple>
#include <utility>
#include <xsparse/util/base_traits.hpp>
#include <xsparse/level_capabilities/coordinate_iterate.hpp>

namespace xsparse
{
    namespace levels
    {
        template <class LowerLevels, class IK, class PK, class OffsetContainer>
        class offset;

        template <class... LowerLevels, class IK, class PK, class OffsetContainer>
        class offset<std::tuple<LowerLevels...>, IK, PK, OffsetContainer>
            : public level_capabilities::coordinate_position_iterate<offset,
                                                                     std::tuple<LowerLevels...>,
                                                                     IK,
                                                                     PK,
                                                                     OffsetContainer>

        {
            using BaseTraits
                = util::base_traits<offset, std::tuple<LowerLevels...>, IK, PK, OffsetContainer>;

        public:
            offset(IK size)
                : m_size(std::move(size))
                , m_offset()
            {
            }

            offset(IK size, OffsetContainer const& offset)
                : m_size(std::move(size))
                , m_offset(offset)
            {
            }

            offset(IK size, OffsetContainer&& offset)
                : m_size(std::move(size))
                , m_offset(offset)
            {
            }

            inline std::pair<PK, PK> pos_bounds(typename BaseTraits::PKM1 pkm1) const noexcept
            {
                return { static_cast<PK>(pkm1), static_cast<PK>(pkm1 + 1) };
            }

            inline IK pos_access([[maybe_unused]] PK pk, typename BaseTraits::I i) const noexcept
            {
                static_assert(std::tuple_size_v<decltype(i)> >= 2,
                              "Tuple size should at least be 2");
                return static_cast<IK>(std::get<0>(i) + m_offset[std::get<1>(i)]);
            }

        private:
            IK m_size;
            OffsetContainer m_offset;
        };
    }  // namespace levels

    template <class... LowerLevels, class IK, class PK, class OffsetContainer>
    struct util::coordinate_position_trait<
        levels::offset<std::tuple<LowerLevels...>, IK, PK, OffsetContainer>>
    {
        using Coordinate = IK;
        using Position = PK;
    };
}  // namespace xsparse


#endif
