require_relative '../mearmwidget'

describe MeArmWidget do
  before :all do
    @app = Qt::Application.new []
  end
  let :client do
    double 'Client'
  end
  let :widget do
    expect(client).to receive(:pos).and_return([1, 2, 3, 4])
    MeArmWidget.new client
  end
  context 'when moving the base slider' do
    it 'should move the base' do
      expect(client).to receive(:ready?).and_return true
      expect(client).to receive(:target).with(10, 2, 3, 4)
      widget.ui.baseSlider.setValue 10
    end
    it 'should start polling if robot not ready' do
      expect(client).to receive(:ready?).and_return false
      expect(widget).to receive(:defer)
      widget.ui.baseSlider.setValue 10
    end
  end
  context 'with pending updates' do
    before :each do
      widget.defer
    end
    it 'should process them when ready' do
      expect(client).to receive(:ready?).and_return true
      expect(client).to receive(:target)
      widget.pending
    end
    it 'should defer them if robot is not ready' do
      widget.defer
      expect(client).to receive(:ready?).and_return false
      expect(widget).to receive(:defer)
      widget.pending
    end
  end
  it 'should use values from the shoulder slider' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 10, 3, 4)
    widget.ui.shoulderSlider.setValue 10
  end
  it 'should use values from the elbow slider' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 2, 10, 4)
    widget.ui.elbowSlider.setValue 10
  end
  it 'should use values from the gripper slider' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 2, 3, 10)
    widget.ui.gripperSlider.setValue 10
  end
end
