require_relative '../mearmwidget'

describe MeArmWidget do
  before :all do
    @app = Qt::Application.new []
    #RSpec::Mocks::setup self
  end
  let :client do
    double 'Client'
  end
  let :widget do
    MeArmWidget.new client
  end
  context 'when moving the base slider' do
    it 'should move the base' do
      expect(client).to receive(:target_angle).with(BASE, 10)
      widget.ui.baseSlider.setValue 10
    end
  end
end
