#pragma once
#ifndef QPL_SIGNAL_HPP
#define QPL_SIGNAL_HPP

namespace qpl {
  template<typename T>
  struct signal {
    T value;

    mutable std::vector<std::function<void(const T&)>> listeners;
    mutable std::vector<std::function<void(void)>> voidListeners;

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

    constexpr void runListeners() const
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

    void addListener(const std::function<void(const T&)>& listener) const
    {
      this->listeners.push_back(listener);
    }
    void addListener(const std::function<void(void)>& listener) const
    {
      this->voidListeners.push_back(listener);
    }
  };

  template <typename ... Args> 
  constexpr auto are_signals()
  {
    return qpl::all_true(qpl::constexpr_apply<qpl::variadic_size<Args...>() - 1>([&](auto i)
      {
        return qpl::is_signal<qpl::variadic_type<i, Args...>>();
      }
    ));
  }

  template <typename ... Args>
  constexpr void effect(Args&&... objects)
  requires
  (
    are_signals<Args...>() &&
    qpl::is_callable<qpl::variadic_type_back<Args...>>()

    /*
    * use this when the active error is no longer hallucinated:

    */
  )
  {
    auto&& function = qpl::variadic_value_back(std::forward<Args>(objects)...);

    constexpr auto size = qpl::variadic_size<Args...>();
    qpl::constexpr_iterate<size - 1>([&](auto i)
    {
      qpl::tuple_value<i>(qpl::auto_tie(std::forward<Args>(objects)...)).addListener(function);
    });
  }
}



#endif
