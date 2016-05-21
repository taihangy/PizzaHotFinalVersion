require 'test_helper'

class SessionsControllerTest < ActionController::TestCase
  test "should get new" do
    get :new
    assert_response :success
  end

  test "should log in" do
    yetaihang = users(:one)
    post :create, name: yetaihang.name, password: "secret"
    assert_redirected_to admin_url
    assert_equal yetaihang.id, session[:user_id]
  end

  test "should not log in" do
    yetaihang = users(:one)
    post :create, name: yetaihang.name, password: "wrong"
    assert_redirected_to login_url
  end

  test "should log out" do
    delete :destroy
    assert_redirected_to store_url
  end

end
