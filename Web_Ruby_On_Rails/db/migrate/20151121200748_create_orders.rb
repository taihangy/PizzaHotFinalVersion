class CreateOrders < ActiveRecord::Migration
  def change
    create_table :orders do |t|
      t.string :name
      t.text :address
      t.string :phone
      t.string :email
      t.string :pay_type

      t.timestamps null: false
    end
  end
end
