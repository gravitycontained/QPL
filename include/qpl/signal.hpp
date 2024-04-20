#pragma once
#ifndef QPL_SIGNAL_HPP
#define QPL_SIGNAL_HPP

namespace qpl {
  template<typename T>
  struct signal {
    T value;

    std::vector<std::function<void(const T&)>> listeners;
    std::vector<std::function<void(void)>> voidListeners;

    constexpr signal(){}
    constexpr signal(const T& value)
    {
      this->value = value;
    }

    constexpr operator T& ()
    {
      return this->value;
    }

    constexpr operator const T& () const
    {
      return this->value;
    }

    constexpr void runListeners()
    {
      for (const auto& listener : this->voidListeners)
        listener();

      for (const auto& listener : this->listeners)
        listener(this->value);
    }

    constexpr signal& operator=(const T& newValue)
    {
      auto before = this->value;
      this->value = newValue;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }
    constexpr signal& operator+=(const T& value)
    {
      auto before = this->value;
      this->value += value;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }
    constexpr signal& operator*=(const T& value)
    {
      auto before = this->value;
      this->value *= value;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }
    constexpr signal& operator/=(const T& value)
    {
      auto before = this->value;
      this->value /= value;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }
    constexpr signal& operator<<=(const T& value)
    {
      auto before = this->value;
      this->value <<= value;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }
    constexpr signal& operator>>=(const T& value)
    {
      auto before = this->value;
      this->value >>= value;
      auto changed = (before != this->value);

      if (changed)
        this->runListeners();

      return *this;
    }

    void addListener(const std::function<void(const T&)>& listener)
    {
      this->listeners.push_back(listener);
    }
    void addListener(const std::function<void(void)>& listener)
    {
      this->voidListeners.push_back(listener);
    }
  };

  template <typename T> requires (qpl::is_tuple<T>())
  constexpr auto are_signals()
  {
    auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
      return (qpl::is_signal<qpl::tuple_type<Ints, T>>() && ...);
    };
    return unpack(std::make_index_sequence<qpl::tuple_size<T>()>());
  }

  template <typename ... Args>
  constexpr void effect(Args&&... objects)
  requires
  (
    are_signals<qpl::variadic_type_splice_back<1, Args...>>() &&
    qpl::is_callable<qpl::variadic_type_back<Args...>>()
  )
  {
    constexpr auto size = qpl::variadic_size<Args...>();
    auto&& back = std::get<size - 1>(std::make_tuple(std::forward<Args>(objects)...));

    qpl::constexpr_iterate<size - 1>([&](auto i)
    {
      std::get<i>(qpl::conditional_reference_tie<0, 1>(std::forward<Args>(objects)...)).addListener(back);
    });
  }
}



#endif
