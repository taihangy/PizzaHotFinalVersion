require 'test_helper'

class UserStoriesTest < ActionDispatch::IntegrationTest
	fixtures :products
  	test "buy a product" do
	  	LineItem.delete_all
	  	Order.delete_all
	  	pizza = products(:pizza1)

	  	# the user goes into store index page
	  	get "/"
	  	assert_response :success
	  	assert_template "index"

	  	# the user add product to cart
	  	xml_http_request :post, '/line_items', product_id: pizza.id
	  	assert_response :success

	  	cart = Cart.find(session[:cart_id])
	  	assert_equal 1, cart.line_items.size
	  	assert_equal pizza, cart.line_items[0].product

	  	# the user checkout
	  	get "/orders/new"
	  	assert_response :success
	  	assert_template "new"

	  	# the user fill the form and redirected to the index page and cart is empty
	  	post_via_redirect "/orders", order: { name: "David", 
	  										  address: "Cornell",
	  										  phone: "1234567",
	  										  email: "david@gmail.com",
	  										  pay_type: "Check"}
	  	assert_response :success
	  	assert_template :index
	  	cart = Cart.find(session[:cart_id])
	  	assert_equal 0, cart.line_items.size

	  	# database contains the order just placed, and a line_item has product pizza
	  	orders = Order.all
	  	assert_equal 1, orders.size

	  	order = orders[0]

	  	assert_equal "David", order.name
	  	assert_equal "Cornell", order.address
	  	assert_equal "1234567", order.phone
	  	assert_equal "david@gmail.com", order.email
	  	assert_equal "Check", order.pay_type

	  	assert_equal 1, order.line_items.size
	  	line_item = order.line_items[0]
	  	assert_equal pizza, line_item.product

	  	# mail is correct
	  	mail = ActionMailer::Base.deliveries.last
	  	assert_equal ["david@gmail.com"], mail.to
	  	assert_equal "Pizza Hot <yetaihang.zju@gmail.com>", mail[:from].value
	  	assert_equal "Pizza Hot receives your order", mail.subject
  	end
end
