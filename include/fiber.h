#ifndef __FIBER_H__
#define __FIBER_H__

#include "thread.h"
#include <ucontext.h>
#include <memory>

namespace ConstantineQAQ
{
/**
 * @file fiber.h
 * @brief 包含Fiber类及其相关函数的声明。
 */

/**
 * @class Fiber
 * @brief 表示一个fiber，它是一个轻量级的执行线程。
 */
class Fiber : public std::enable_shared_from_this<Fiber>{
public:
    typedef std::shared_ptr<Fiber> ptr;

    /**
     * @brief 枚举表示fiber的可能状态。
     */
    enum State
    {
        INIT,   /**< fiber已初始化。 */
        HOLD,   /**< fiber当前处于暂停状态。 */
        EXEC,   /**< fiber当前正在执行。 */
        TERM,   /**< fiber已终止。 */
        READY,  /**< fiber准备执行。 */
        EXCEPT  /**< fiber遇到了异常。 */
    };

private:
    /**
     * @brief Fiber类的默认构造函数。
     */
    Fiber();

public:
    /**
     * @brief Fiber类的构造函数。
     * @param cb 由fiber执行的函数。
     * @param stacksize fiber的堆栈大小。
     * @param use_caller 是否使用调用者的堆栈为fiber。
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);

    /**
     * @brief Fiber类的析构函数。
     */
    ~Fiber();

    /**
     * @brief 重置协程函数，并重置状态
     * @param cb 协程执行函数
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 切换到当前协程执行
     */
    void swapIn();

    /**
     * @brief 切换到后台执行
     */
    void swapOut();
    
    /**
     * @brief 将当前线程切换到执行状态
     * @pre 执行的为当前线程的主协程
     */
    void call();

    /**
     * @brief 将当前线程切换到后台
     * @pre 执行的为该协程
     * @post 返回到线程的主协程
     */
    void back();
    /**
     * @brief 获取fiber的ID。
     * @return fiber的ID。
     */
    uint64_t getId() const { return m_id; }

    /**
     * @brief 获取fiber的状态。
     * @return fiber的状态。
     */
    State getState() const { return m_state; }

    /**
     * @brief 设置fiber的状态。
     * @param state fiber的新状态。
     */
    void setState(State state) { m_state = state; }

    /**
     * @brief 设置当前fiber。
     * @param f 新的当前fiber。
     */
    static void SetThis(Fiber* f);

    /**
     * @brief 返回当前协程
     * @return 当前协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 协程切换到后台，并且设置为Ready状态
     */
    static void YieldToReady();

    /**
     * @brief 协程切换到后台，并且设置为Hold状态
     */
    static void YieldToHold();

    /**
     * @brief 总协程数
     * @return 协程数
     */
    static uint64_t TotalFibers();

    /**
     * @brief fiber的主函数。
     */
    static void MainFunc();

    /**
     * @brief fiber的调用者的主函数。
     */
    static void CallerMainFunc();

    /**
     * @brief 获取当前fiber的ID。
     * @return 当前fiber的ID。
     */
    static uint64_t GetFiberId();

private:
    uint64_t m_id = 0; /**< fiber的ID。 */
    uint32_t m_stacksize = 0; /**< fiber的堆栈大小。 */
    State m_state = INIT; /**< fiber的状态。 */

    ucontext_t m_ctx; /**< fiber的上下文。 */
    void* m_stack = nullptr; /**< fiber的堆栈。 */

    std::function<void()> m_cb; /**< 由fiber执行的函数。 */
};
} // namespace ConstantineQAQ

#endif
