class AddCatagoryToProducts < ActiveRecord::Migration
  def change
    add_column :products, :catagory, :string
  end
end
