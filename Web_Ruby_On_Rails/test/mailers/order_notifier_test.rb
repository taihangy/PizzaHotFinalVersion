require 'test_helper'

class OrderNotifierTest < ActionMailer::TestCase
  test "received" do
    mail = OrderNotifier.received(orders(:one))
    assert_equal "Pizza Hot receives your order", mail.subject
    assert_equal ["yetaihang@example.org"], mail.to
    assert_equal ["yetaihang.zju@gmail.com"], mail.from
  end

  test "shipped" do
    mail = OrderNotifier.shipped(orders(:one))
    assert_equal "Pizza Hot is now shipping your order", mail.subject
    assert_equal ["yetaihang@example.org"], mail.to
    assert_equal ["yetaihang.zju@gmail.com"], mail.from
  end
end
