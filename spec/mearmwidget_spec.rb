require_relative '../mearmwidget'

describe MeArmWidget do
  before :all do
    @app = Qt::Application.new []
  end
  let :client do
    double 'Client'
  end
  let :widget do
    MeArmWidget.new client
  end
  context 'when moving the base slider' do
    it 'should move the base' do
      expect(client).to receive(:ready?).and_return true
      expect(client).to receive(:target).with(10)
      widget.ui.baseSlider.setValue 10
    end
    it 'should start polling if robot not ready' do
      expect(client).to receive(:ready?).and_return false
      expect(widget).to receive(:defer)
      widget.ui.baseSlider.setValue 10
    end
  end
  it 'should process pending updates' do
    widget.defer
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target)
    widget.pending
  end
  it 'should defer pending updates if robot is not ready' do
    widget.defer
    expect(client).to receive(:ready?).and_return false
    expect(widget).to receive(:defer)
    widget.pending
  end
end
