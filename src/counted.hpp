#ifndef COUNTED_HPP
#define COUNTED_HPP

template <typename Derived>
class counted {
private:
    static inline int count_ = 0;

public:
    counted() {
        ++count_;
    }

    counted(const counted&) {
        ++count_;
    }

    counted(counted&&) noexcept {
        ++count_;
    }

    ~counted() {
        --count_;
    }

    static int alive() {
        return count_;
    }
};

#endif
